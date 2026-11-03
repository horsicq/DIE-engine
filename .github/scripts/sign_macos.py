#!/usr/bin/env python3
"""Sign and notarize macOS ZIP and PKG outputs for trusted CI builds."""

import argparse
import base64
import binascii
from datetime import datetime, timezone
import hashlib
import json
import os
from pathlib import Path
import plistlib
import re
import secrets
import shlex
import shutil
import signal
import subprocess
import sys
import tempfile
import time


CERTIFICATE_FIELDS = (
    "APPLE_CERTIFICATE_P12_BASE64",
    "APPLE_CERTIFICATE_PASSWORD",
    "APPLE_INSTALLER_CERTIFICATE_P12_BASE64",
    "APPLE_INSTALLER_CERTIFICATE_PASSWORD",
)
API_KEY_FIELDS = (
    "APPLE_NOTARY_KEY_P8",
    "APPLE_NOTARY_KEY_ID",
    "APPLE_NOTARY_ISSUER_ID",
)
APPLE_ID_FIELDS = (
    "APPLE_NOTARY_APPLE_ID",
    "APPLE_NOTARY_PASSWORD",
    "APPLE_NOTARY_TEAM_ID",
)
CORE_CREDENTIAL_FIELDS = CERTIFICATE_FIELDS + API_KEY_FIELDS + APPLE_ID_FIELDS
MACH_O_MAGIC = {
    b"\xfe\xed\xfa\xce", b"\xce\xfa\xed\xfe",
    b"\xfe\xed\xfa\xcf", b"\xcf\xfa\xed\xfe",
    b"\xca\xfe\xba\xbe", b"\xbe\xba\xfe\xca",
    b"\xca\xfe\xba\xbf", b"\xbf\xba\xfe\xca",
}
SIGNING_TIMEOUT = 70 * 60  # Leave five minutes for cleanup before the CI step limit.
NOTARY_POLL_TIMEOUT = 30 * 60
NOTARY_POLL_INTERVAL = 15
NOTARY_REQUEST_TIMEOUT = 60
NOTARY_STATUS_FAILURES = 5
NOTARY_LOG_ATTEMPTS = 3
_signing_deadline = None


class CommandTimeout(RuntimeError):
    def __init__(self, message, *, stdout="", stderr=""):
        super().__init__(message)
        # Keep partial output available to recover a submission ID. Never put
        # credential-bearing tool output in the exception message or CI logs.
        self.stdout = stdout
        self.stderr = stderr


class SigningTimeout(RuntimeError):
    pass


class SigningInterrupted(RuntimeError):
    pass


def remaining_timeout(timeout):
    if _signing_deadline is None:
        return timeout
    remaining = _signing_deadline - time.monotonic()
    if remaining <= 0:
        raise SigningTimeout("Signing exceeded its 70-minute budget; removing temporary credentials.")
    return min(timeout, remaining)


def redacted(text):
    if isinstance(text, bytes):
        text = text.decode("utf-8", errors="replace")
    text = text or ""
    values = {os.environ.get(name, "").strip() for name in CORE_CREDENTIAL_FIELDS}
    for value in sorted(values - {""}, key=len, reverse=True):
        text = text.replace(value, "[REDACTED]")
    return text


def command_label(args):
    program = Path(str(args[0])).name
    if program == "xcrun" and len(args) >= 3 and args[1] in ("stapler", "notarytool"):
        return " ".join(args[1:3])
    if program == "security" and len(args) >= 2:
        return "security " + args[1]
    return program


def stop_process(process):
    # Apple tools can launch helpers. Stop the whole group before retrying or
    # removing its keychain, including when GitHub cancels the signing step.
    try:
        os.killpg(process.pid, signal.SIGKILL)
    except ProcessLookupError:
        pass
    try:
        return process.communicate(timeout=5)
    except subprocess.TimeoutExpired:
        # A daemon may have inherited a pipe without remaining in our group.
        process.stdout.close()
        process.stderr.close()
        try:
            process.wait(timeout=5)
        except subprocess.TimeoutExpired:
            raise RuntimeError("Could not stop a signing subprocess within five seconds.") from None
        return "", ""


def capture_process_diagnostics(process, directory, *, timeout):
    """Sample one signing process, without collecting arguments or environment."""
    directory.mkdir(parents=True, exist_ok=True)
    deadline = time.monotonic() + timeout
    report = {"pid": process.pid, "status": "partial", "tools": {}}
    report_path = directory / "process.json"
    report_path.write_text(json.dumps(report, indent=2))
    diagnostic_env = {name: value for name, value in os.environ.items()
                      if not name.startswith("APPLE_")}

    def collect(name, args, raw_file=None):
        available = deadline - time.monotonic()
        if available <= 0:
            report["tools"][name] = {"status": "skipped", "reason": "Diagnostic time budget exhausted."}
            return
        try:
            result = run(*args, check=False, env=diagnostic_env, timeout=min(15, available),
                         phase=f"installer diagnostics: {name}")
        except (SigningInterrupted, SigningTimeout):
            raise
        except (RuntimeError, OSError) as error:
            report["tools"][name] = {"status": "failed", "error": redacted(str(error))}
            print(f"::warning::Could not collect {name} process diagnostics.", flush=True)
        else:
            report["tools"][name] = {"status": "succeeded" if result.returncode == 0 else "failed",
                                      "returncode": result.returncode}
            output_name = f"{name}-output.txt" if raw_file is not None else f"{name}.txt"
            (directory / output_name).write_text(redacted(result.stdout) + redacted(result.stderr))
            if result.returncode:
                print(f"::warning::{name} process diagnostics exited with code {result.returncode}.", flush=True)
        finally:
            # Raw stack reports stay outside the uploaded artifact tree until
            # redacted. Preserve a partial report even if the sampler times out.
            if raw_file is not None and raw_file.is_file():
                (directory / f"{name}.txt").write_text(redacted(raw_file.read_text(errors="replace")))
            report_path.write_text(json.dumps(report, indent=2))

    # start_new_session makes the target PID its process group ID. `comm`
    # deliberately excludes command arguments, which may contain credentials.
    collect("ps", ("/bin/ps", "-g", str(process.pid), "-x", "-o",
                   "pid=,ppid=,pgid=,stat=,etime=,time=,comm="))
    if sys.platform == "darwin" and process.poll() is None:
        # Unlike a whole-system spindump, sample targets only our productbuild
        # process. Permission failures are diagnostic results, not signing failures.
        with tempfile.TemporaryDirectory(prefix="die-process-sample-", dir=os.environ["RUNNER_TEMP"]) as scratch:
            raw_file = Path(scratch) / "sample.txt"
            collect("sample", ("/usr/bin/sample", str(process.pid), "5", "10", "-mayDie", "-file", str(raw_file)), raw_file)
    else:
        report["tools"]["sample"] = {"status": "skipped", "reason": "Requires a running macOS process."}
    if all(tool["status"] == "succeeded" for tool in report["tools"].values()):
        report["status"] = "captured"
    report_path.write_text(json.dumps(report, indent=2))


def run(*args, check=True, env=None, timeout=120, phase=None, diagnose_after=()):
    # Never echo commands: security/notarytool arguments contain credentials.
    label = phase or command_label(args)
    timeout = remaining_timeout(timeout)
    started = time.monotonic()
    deadline = started + timeout
    print(f"Starting {label} (timeout {timeout:.0f}s)...", flush=True)
    diagnostics = Path(os.environ["RUNNER_TEMP"]) / "die-apple-notarization" / "commands"
    diagnostics.mkdir(parents=True, exist_ok=True)
    record_path = diagnostics / f"{time.time_ns()}.json"
    record = {"phase": label, "status": "started", "timeout_seconds": timeout,
              "started_at": datetime.now(timezone.utc).isoformat()}
    record_path.write_text(json.dumps(record, indent=2))
    safe_output = (Path(str(args[0])).name in ("codesign", "ditto", "spctl", "productbuild", "pkgutil", "diec")
                   or args[:2] == ("xcrun", "stapler"))

    def finish(status, stdout="", stderr="", returncode=None):
        record.update(status=status, elapsed_seconds=round(time.monotonic() - started, 2),
                      returncode=returncode, finished_at=datetime.now(timezone.utc).isoformat())
        if safe_output:
            record.update(stdout=redacted(stdout), stderr=redacted(stderr))
            if status != "succeeded" or args[:2] == ("xcrun", "stapler"):
                for output in (record["stdout"], record["stderr"]):
                    for line in output.splitlines():
                        print("  " + line, file=sys.stderr, flush=True)
        record_path.write_text(json.dumps(record, indent=2))
        print(f"Finished {label}: {status} ({record['elapsed_seconds']:.1f}s).", flush=True)

    try:
        process = subprocess.Popen([str(arg) for arg in args], text=True,
                                   stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                   env=env, start_new_session=True)
    except OSError:
        finish("failed")
        raise RuntimeError(f"Could not start {label}.") from None
    try:
        # Sampling time counts against the original command deadline; it must
        # never restart or extend the 15-minute PKG limit or total signing budget.
        completed = False
        checkpoints = [point for point in sorted(set(diagnose_after)) if 0 < point < timeout]
        for point in checkpoints:
            until_checkpoint = max(0, started + point - time.monotonic())
            try:
                stdout, stderr = process.communicate(timeout=until_checkpoint)
                completed = True
                break
            except subprocess.TimeoutExpired:
                if process.poll() is not None:
                    continue
                available = deadline - time.monotonic()
                if available <= 0:
                    break
                sample_dir = diagnostics.parent / "processes" / record_path.stem / f"after-{point:g}s"
                print(f"{label} is still running after {time.monotonic() - started:.0f}s; collecting process diagnostics.", flush=True)
                record.setdefault("process_diagnostics", []).append(str(sample_dir.relative_to(diagnostics.parent)))
                record_path.write_text(json.dumps(record, indent=2))
                try:
                    capture_process_diagnostics(process, sample_dir, timeout=min(20, available))
                except (SigningInterrupted, SigningTimeout):
                    raise
                except (RuntimeError, OSError) as error:
                    # A failed profiler must not cancel otherwise valid signing.
                    print(f"::warning::Process diagnostics unavailable: {redacted(str(error))}", flush=True)
        # A checkpoint may consume the remaining command budget. Always obtain
        # the final result (or timeout) rather than treating a sample as completion.
        if not completed:
            stdout, stderr = process.communicate(timeout=max(0, deadline - time.monotonic()))
    except subprocess.TimeoutExpired as error:
        stdout, stderr = stop_process(process)
        finish("timed_out", stdout or error.stdout, stderr or error.stderr, process.returncode)
        raise CommandTimeout(f"{label} timed out after {timeout:.0f}s.",
                             stdout=stdout or error.stdout, stderr=stderr or error.stderr) from None
    except BaseException:
        stdout, stderr = stop_process(process)
        finish("interrupted", stdout, stderr, process.returncode)
        raise
    result = subprocess.CompletedProcess(args, process.returncode, stdout, stderr)
    finish("succeeded" if result.returncode == 0 else "failed", stdout, stderr, result.returncode)
    if check and result.returncode:
        raise RuntimeError(f"{label} failed (exit {result.returncode}).")
    return result


def stapler(action, target, *, kind):
    # Retry ticket-service/network failures, never invalid or revoked tickets.
    # Validation still has to succeed for both the app and the installer.
    for attempt in range(1, 4):
        phase = f"{kind}: stapler {action}, attempt {attempt}/3"
        try:
            result = run("xcrun", "stapler", action, target, check=False, timeout=120, phase=phase)
        except CommandTimeout as error:
            failure = str(error)
        else:
            if result.returncode == 0:
                return
            failure = f"{phase} failed (exit {result.returncode})."
            if result.returncode not in (68, 75):
                raise RuntimeError(failure)
        if attempt == 3:
            raise RuntimeError(failure + " All stapler attempts failed; see the notarization diagnostics.")
        delay = remaining_timeout(15 * attempt)
        print(f"::warning::{failure} Retrying in {delay:.0f}s.", flush=True)
        time.sleep(delay)


def has_fields(names):
    return all(os.environ.get(name, "").strip() for name in names)


def notary_mode():
    if has_fields(API_KEY_FIELDS):
        return "api-key"
    if has_fields(APPLE_ID_FIELDS):
        return "apple-id"
    return None


def check_config(allow_unsigned=False):
    configured = any(os.environ.get(name, "").strip() for name in CORE_CREDENTIAL_FIELDS)
    if allow_unsigned and not configured:
        return False
    missing = [name for name in CERTIFICATE_FIELDS if not os.environ.get(name, "").strip()]
    if missing:
        raise RuntimeError("Missing Apple signing configuration: " + ", ".join(missing))
    if notary_mode() is None:
        raise RuntimeError("Incomplete Apple notarization configuration: supply all of "
                           + ", ".join(API_KEY_FIELDS) + "; or all of "
                           + ", ".join(APPLE_ID_FIELDS) + " (with an app-specific password).")
    return True


def report_config(enabled):
    if not enabled:
        message = "Apple signing credentials are not configured; this run will produce unsigned artifacts."
        print("::notice::" + message)
        summary = os.environ.get("GITHUB_STEP_SUMMARY")
        if summary:
            with open(summary, "a", encoding="utf-8") as output:
                output.write(message + "\n")
    output_path = os.environ.get("GITHUB_OUTPUT")
    if output_path:
        with open(output_path, "a", encoding="utf-8") as output:
            output.write("enabled=" + str(enabled).lower() + "\n")


def notary_auth(root):
    mode = notary_mode()
    if mode == "api-key":
        key = root / "AuthKey.p8"
        key.write_text(os.environ["APPLE_NOTARY_KEY_P8"].strip() + "\n", encoding="utf-8")
        return ["--key", key, "--key-id", os.environ["APPLE_NOTARY_KEY_ID"],
                "--issuer", os.environ["APPLE_NOTARY_ISSUER_ID"]]
    if mode == "apple-id":
        return ["--apple-id", os.environ["APPLE_NOTARY_APPLE_ID"],
                "--password", os.environ["APPLE_NOTARY_PASSWORD"],
                "--team-id", os.environ["APPLE_NOTARY_TEAM_ID"]]
    raise RuntimeError("A complete API key or Apple ID notarization configuration is required.")


def signing_dir():
    return Path(os.environ["RUNNER_TEMP"]) / "die-apple-signing"


def cleanup():
    root = signing_dir()
    if not root.exists():
        return
    errors = []
    try:
        saved = root / "original-keychains.json"
        if saved.exists():
            originals = json.loads(saved.read_text())
            try:
                run("security", "list-keychains", "-d", "user", "-s", *originals, timeout=30,
                    phase="Restore original keychain search list")
            except RuntimeError as error:
                errors.append(str(error))
        keychain = root / "signing.keychain-db"
        if keychain.exists():
            try:
                run("security", "delete-keychain", keychain, timeout=30,
                    phase="Delete temporary signing keychain")
            except RuntimeError as error:
                errors.append(str(error))
    finally:
        # Also remove the private key/certificate if security itself failed.
        shutil.rmtree(root)
    if errors:
        raise RuntimeError("Signing cleanup failed: " + "; ".join(errors))


def select_identity(output, kind, selector):
    identities = re.findall(
        rf'\b([0-9A-Fa-f]{{40}}) "(Developer ID {kind}:[^"\n]+)"', output,
    )
    requested = os.environ.get(selector, "").strip()
    if requested:
        identities = [(fingerprint, name) for fingerprint, name in identities
                      if requested == name or requested.upper() == fingerprint.upper()]
    if len(identities) != 1:
        raise RuntimeError(f"Expected one valid Developer ID {kind} identity; "
                           f"check the .p12 and optional {selector}.")
    return identities[0]


def import_identities(root):
    keychain = root / "signing.keychain-db"
    password = secrets.token_hex(32)
    original = shlex.split(run("security", "list-keychains", "-d", "user").stdout)
    (root / "original-keychains.json").write_text(json.dumps(original))
    run("security", "create-keychain", "-p", password, keychain)
    run("security", "set-keychain-settings", "-lut", "21600", keychain)
    run("security", "unlock-keychain", "-p", password, keychain)
    for prefix, name, tool in (
        ("APPLE_CERTIFICATE", "application.p12", "/usr/bin/codesign"),
        ("APPLE_INSTALLER_CERTIFICATE", "installer.p12", "/usr/bin/productbuild"),
    ):
        certificate = root / name
        field = prefix + "_P12_BASE64"
        try:
            encoded = "".join(os.environ[field].split())
            certificate.write_bytes(base64.b64decode(encoded, validate=True))
        except (ValueError, binascii.Error):
            raise RuntimeError(f"{field} is not valid base64.") from None
        run("security", "import", certificate, "-k", keychain, "-P",
            os.environ[prefix + "_PASSWORD"], "-t", "cert", "-f", "pkcs12", "-T", tool)
    run("security", "set-key-partition-list", "-S", "apple-tool:,apple:",
        "-k", password, keychain)
    run("security", "list-keychains", "-d", "user", "-s", keychain, *original)
    application = select_identity(
        run("security", "find-identity", "-v", "-p", "codesigning", keychain).stdout,
        "Application", "APPLE_SIGNING_IDENTITY",
    )
    # Installer certificates are excluded by the codesigning policy. Use the
    # default basic X.509 policy, and pass productbuild the full Common Name.
    installer = select_identity(
        run("security", "find-identity", "-v", keychain).stdout,
        "Installer", "APPLE_INSTALLER_SIGNING_IDENTITY",
    )
    return keychain, application[0], installer[1]


def app_in(package):
    apps = list(package.glob("*.app"))
    if len(apps) != 1 or not apps[0].is_dir():
        raise RuntimeError("Expected exactly one app in die_mac_portable.")
    return apps[0]


def sign_app(app, root, keychain, identity):
    arch = os.environ["CMAKE_OSX_ARCHITECTURES"]
    if arch not in ("x86_64", "arm64"):
        raise RuntimeError("Unsupported Apple architecture.")
    entitlements = {"com.apple.security.cs.allow-jit": True}
    if arch == "x86_64":
        # Qt 6.8.3's Intel JIT capability probe uses RWX mmap without MAP_JIT.
        entitlements["com.apple.security.cs.allow-unsigned-executable-memory"] = True
    entitlements_file = root / "executable-entitlements.plist"
    entitlements_file.write_bytes(plistlib.dumps(entitlements))
    info = plistlib.loads((app / "Contents/Info.plist").read_bytes())
    executable = app / "Contents/MacOS" / info["CFBundleExecutable"]
    diec = app / "Contents/MacOS/diec"
    if not executable.is_file() or not diec.is_file():
        raise RuntimeError("The package must contain both GUI and diec executables.")
    command = ["codesign", "--force", "--sign", identity, "--keychain", keychain,
               "--options", "runtime", "--timestamp"]

    # Sign inside-out. Do not use --deep for signing or grant entitlements to
    # libraries. Framework directory signatures seal their resources as well.
    paths = sorted(app.rglob("*"), key=lambda path: len(path.parts), reverse=True)
    for path in paths:
        if path.is_symlink() or path in (executable, diec):
            continue
        if path.is_file():
            with path.open("rb") as source:
                is_macho = source.read(4) in MACH_O_MAGIC
            if is_macho:
                run(*command, path)
        elif path.suffix == ".framework":
            run(*command, path)
    run(*command, "--identifier", info["CFBundleIdentifier"] + ".diec",
        "--entitlements", entitlements_file, diec)
    run(*command, "--entitlements", entitlements_file, app)
    run("codesign", "--verify", "--deep", "--strict", "--verbose=2", app)
    run("codesign", "--verify", "--strict", "--verbose=2", diec)


def notary_json(output):
    try:
        value = json.loads(output)
        return value if isinstance(value, dict) else {}
    except (ValueError, TypeError):
        return {}


def valid_submission_id(value):
    return isinstance(value, str) and re.fullmatch(
        r"[0-9a-fA-F]{8}(?:-[0-9a-fA-F]{4}){3}-[0-9a-fA-F]{12}", value) is not None


def write_notary_json(path, value):
    temporary = path.with_suffix(".tmp")
    temporary.write_text(json.dumps(value, indent=2))
    temporary.replace(path)


def restore_matching_notary_state(digest, diagnostics, kind):
    # The CI restore step validates provenance and collects earlier attempts in
    # a separate hash-indexed directory. A rebuilt/re-signed file may differ;
    # only identical uploaded bytes are eligible for the earlier submission ID.
    restored_file = Path(os.environ["RUNNER_TEMP"]) / "die-apple-notary-restored" / kind / f"{digest}.json"
    if not restored_file.exists() and not restored_file.is_symlink():
        return
    if restored_file.is_symlink() or restored_file.stat().st_size > 16 * 1024:
        raise RuntimeError(f"Invalid restored {kind} notarization state; refusing to upload again.")
    restored = notary_json(restored_file.read_text())
    if (restored.get("sha256") != digest
            or restored.get("upload_state") not in ("submitted", "uncertain")
            or ("id" in restored and not valid_submission_id(restored["id"]))
            or ("id" not in restored and restored.get("upload_state") != "uncertain")
            or ("status" in restored and restored["status"] not in ("In Progress", "Accepted", "Invalid", "Rejected"))):
        raise RuntimeError(f"Invalid restored {kind} notarization state; refusing to upload again.")
    # Never promote arbitrary artifact fields into the current diagnostics.
    state = {name: restored[name] for name in ("sha256", "upload_state", "id", "status") if name in restored}
    write_notary_json(diagnostics / "submission.json", state)
    print(f"Restored {kind} notarization state for matching SHA-256 {digest}.", flush=True)


def prepare_notary_submission(submission, auth, diagnostics, kind):
    hasher = hashlib.sha256()
    with submission.open("rb") as source:
        for block in iter(lambda: source.read(1024 * 1024), b""):
            hasher.update(block)
    digest = hasher.hexdigest()
    state_file = diagnostics / "submission.json"
    if not state_file.exists():
        restore_matching_notary_state(digest, diagnostics, kind)
    if state_file.exists():
        state = notary_json(state_file.read_text())
        if state.get("sha256") != digest:
            raise RuntimeError(f"Saved {kind} notarization state belongs to a different file; refusing to reuse or replace it.")
        if not valid_submission_id(state.get("id")):
            raise RuntimeError(f"The previous {kind} upload has no usable submission ID; "
                               "Apple may have received it. Automatic resubmission is disabled.")
        print(f"Resuming {kind} notarization with saved submission ID {state['id']}.", flush=True)
        return state

    # Record the attempt before making a request: losing its response must not
    # cause a second upload of the same bytes on a subsequent invocation.
    state = {"sha256": digest, "upload_state": "uncertain",
             "upload_started_at": datetime.now(timezone.utc).isoformat()}
    write_notary_json(state_file, state)
    print(f"Uploading signed {kind} to Apple notarization once...", flush=True)
    try:
        result = run("xcrun", "notarytool", "submit", submission, *auth,
                     "--no-wait", "--output-format", "json", check=False,
                     timeout=35 * 60, phase=f"{kind}: upload notarization submission")
        response = notary_json(result.stdout)
        state["upload_returncode"] = result.returncode
    except CommandTimeout as error:
        response = notary_json(error.stdout)
        state["upload_timed_out"] = True
    submission_id = response.get("id")
    if valid_submission_id(submission_id):
        state["id"] = submission_id.lower()
        if state.get("upload_returncode") == 0:
            state["upload_state"] = "submitted"
    # Persist the ID before any status query, even if submit ended with an error.
    # Such an ID is useful for recovery but is not proof of a completed upload.
    write_notary_json(state_file, state)
    if not valid_submission_id(state.get("id")):
        raise RuntimeError(f"The {kind} upload ended without a usable submission ID; "
                           "Apple may have received it. Automatic resubmission is disabled. "
                           "See submission.json in the notarization diagnostics.")
    print(f"Saved {kind} notarization submission ID {state['id']}.", flush=True)
    return state


def poll_notary_submission(state, auth, diagnostics, kind):
    submission_id = state["id"]
    deadline = time.monotonic() + NOTARY_POLL_TIMEOUT
    failures = 0
    attempt = 0
    while True:
        available = deadline - time.monotonic()
        if available <= 0:
            raise RuntimeError(f"Timed out checking {kind} notarization {submission_id}. "
                               "The submission ID is saved; no file was resubmitted.")
        attempt += 1
        response = {}
        try:
            result = run("xcrun", "notarytool", "info", submission_id, *auth,
                         "--output-format", "json", check=False,
                         timeout=remaining_timeout(min(NOTARY_REQUEST_TIMEOUT, available)),
                         phase=f"{kind}: check notarization {submission_id}, attempt {attempt}")
        except CommandTimeout:
            pass
        else:
            if result.returncode == 0:
                response = notary_json(result.stdout)
        response_id = response.get("id")
        if valid_submission_id(response_id) and response_id.lower() != submission_id.lower():
            raise RuntimeError(f"Apple returned a different submission ID while checking {kind}; refusing to use its status.")
        status = response.get("status")
        if valid_submission_id(response_id) and status in ("In Progress", "Accepted", "Invalid", "Rejected"):
            failures = 0
            state.update(status=status, status_checked_at=datetime.now(timezone.utc).isoformat())
            write_notary_json(diagnostics / "submission.json", state)
            print(f"{kind} notarization {submission_id}: {status}.", flush=True)
            if status != "In Progress":
                return status
            delay = NOTARY_POLL_INTERVAL
        else:
            failures += 1
            if failures >= NOTARY_STATUS_FAILURES:
                raise RuntimeError(f"Could not retrieve {kind} notarization status for {submission_id} "
                                   f"after {failures} consecutive failures. The ID is saved; no file was resubmitted.")
            delay = min(15 * 2 ** (failures - 1), 60)
            print(f"::warning::Status request failed for {kind} notarization {submission_id}; "
                  "retrying the same submission ID.", flush=True)
        available = deadline - time.monotonic()
        if available > 0:
            time.sleep(remaining_timeout(min(delay, available)))


def fetch_notary_log(submission_id, auth, diagnostics, kind):
    log_file = diagnostics / "notarization-log.json"
    partial = diagnostics / "notarization-log.partial"
    try:
        for attempt in range(1, NOTARY_LOG_ATTEMPTS + 1):
            partial.unlink(missing_ok=True)
            try:
                result = run("xcrun", "notarytool", "log", submission_id, *auth, partial,
                             check=False, timeout=NOTARY_REQUEST_TIMEOUT,
                             phase=f"{kind}: retrieve notarization log {submission_id}, attempt {attempt}")
            except CommandTimeout:
                pass
            else:
                if result.returncode == 0 and partial.is_file():
                    log = notary_json(partial.read_text(errors="replace"))
                    log_id = log.get("jobId")
                    if valid_submission_id(log_id) and log_id.lower() == submission_id.lower():
                        write_notary_json(log_file, log)
                        return
            if attempt < NOTARY_LOG_ATTEMPTS:
                print(f"::warning::Could not fetch the {kind} notarization log; retrying ID {submission_id}.", flush=True)
                time.sleep(remaining_timeout(15 * attempt))
        print(f"::warning::Apple's {kind} notarization log is unavailable after {NOTARY_LOG_ATTEMPTS} attempts. "
              f"Submission ID {submission_id} is saved for later retrieval.", flush=True)
    finally:
        partial.unlink(missing_ok=True)


def notarize_submission(submission, target, root, kind):
    diagnostics = Path(os.environ["RUNNER_TEMP"]) / "die-apple-notarization" / kind
    diagnostics.mkdir(parents=True, exist_ok=True)
    auth = notary_auth(root)
    state = prepare_notary_submission(submission, auth, diagnostics, kind)
    status = poll_notary_submission(state, auth, diagnostics, kind)
    # Only completed submissions have logs. A log outage must not undo a
    # separately confirmed acceptance, nor turn a rejection into acceptance.
    fetch_notary_log(state["id"], auth, diagnostics, kind)
    if status != "Accepted":
        raise RuntimeError("Apple did not accept this submission. "
                           "See the notarization diagnostic artifact.")
    print(f"Apple accepted {kind} notarization; attaching and validating the ticket.", flush=True)
    stapler("staple", target, kind=kind)
    stapler("validate", target, kind=kind)


def notarize(app, root):
    submission = root / "submission.zip"
    run("ditto", "-c", "-k", "--sequesterRsrc", "--keepParent", app, submission,
        timeout=300, phase="app: create notarization ZIP")
    notarize_submission(submission, app, root, "app")
    run("codesign", "--verify", "--deep", "--strict", "--verbose=2", app,
        phase="app: verify signature after stapling")
    run("spctl", "--assess", "--type", "execute", "--verbose=2", app,
        phase="app: Gatekeeper assessment")


def notarize_installer(installer, root):
    notarize_submission(installer, installer, root, "installer")
    run("pkgutil", "--check-signature", installer, phase="installer: verify signature after stapling")
    run("spctl", "--assess", "--type", "install", "--verbose=2", installer,
        phase="installer: Gatekeeper assessment")


def require_output(path):
    if not path.is_file() or not path.stat().st_size:
        raise RuntimeError(f"Package tool did not create a nonempty output: {path.name}")


def sign_package(archive, installer):
    global _signing_deadline
    check_config()
    archive = archive.resolve(strict=True)
    installer = installer.resolve(strict=True)
    if archive.suffix != ".zip" or installer.suffix != ".pkg":
        raise RuntimeError("Signing requires a ZIP archive and a PKG installer.")
    require_output(archive)
    require_output(installer)
    root = signing_dir()
    # Refuse to reuse leftover credentials from another invocation.
    root.mkdir(mode=0o700)
    temporary_zip = archive.with_suffix(".signed.zip")
    temporary_pkg = installer.with_suffix(".signed.pkg")
    _signing_deadline = time.monotonic() + SIGNING_TIMEOUT
    try:
        temporary_zip.unlink(missing_ok=True)
        temporary_pkg.unlink(missing_ok=True)
        keychain, identity, installer_identity = import_identities(root)
        extracted = root / "unpacked"
        run("ditto", "-x", "-k", archive, extracted, timeout=300, phase="Extract unsigned package")
        package = extracted / "die_mac_portable"
        app = app_in(package)
        sign_app(app, root, keychain, identity)
        notarize(app, root)
        run("ditto", "-c", "-k", "--sequesterRsrc", "--keepParent", package, temporary_zip,
            timeout=300, phase="Repack signed and stapled app")
        require_output(temporary_zip)
        # Validate the actual redistributable bytes after repacking the ticket.
        verified = root / "verified"
        run("ditto", "-x", "-k", temporary_zip, verified, timeout=300, phase="Extract final ZIP for verification")
        final_app = app_in(verified / "die_mac_portable")
        run("codesign", "--verify", "--deep", "--strict", "--verbose=2", final_app,
            phase="final ZIP: verify app signature")
        stapler("validate", final_app, kind="final ZIP app")
        run("spctl", "--assess", "--type", "execute", "--verbose=2", final_app,
            phase="final ZIP: Gatekeeper assessment")
        # Run the signed CLI without Qt paths from the build environment or
        # signing credentials; this checks loading under the hardened runtime.
        runtime_env = {name: value for name, value in os.environ.items()
                       if not name.startswith(("APPLE_", "DYLD_", "QT_", "QML"))}
        run(final_app / "Contents/MacOS/diec", "--version", env=runtime_env,
            timeout=60, phase="final ZIP: run signed diec --version")
        # Rebuild from the signed and stapled app: signing the original PKG
        # would leave its application payload unsigned.
        run("productbuild", "--component", app, "/Applications", "--sign",
            installer_identity, "--keychain", keychain, "--timestamp", temporary_pkg,
            timeout=900, phase="installer: build signed PKG", diagnose_after=(60, 300, 840))
        require_output(temporary_pkg)
        run("pkgutil", "--check-signature", temporary_pkg, phase="installer: verify signature before submission")
        notarize_installer(temporary_pkg, root)
        # Replace unsigned outputs only after both pass every check.
        temporary_zip.replace(archive)
        temporary_pkg.replace(installer)
        print("Signed, notarized, and stapled ZIP and PKG verified.", flush=True)
    finally:
        # Cleanup gets its own short command limits even after our total budget
        # expires. GitHub's always() cleanup step is a second line of defense.
        _signing_deadline = None
        try:
            temporary_zip.unlink(missing_ok=True)
            temporary_pkg.unlink(missing_ok=True)
        finally:
            cleanup()


def interrupted(signum, frame):
    raise SigningInterrupted("Signing was interrupted; removing temporary credentials.")


def main():
    os.umask(0o077)
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("action", choices=("check", "sign", "cleanup"))
    parser.add_argument("archive", nargs="?", type=Path)
    parser.add_argument("installer", nargs="?", type=Path)
    parser.add_argument("--allow-unsigned", action="store_true",
                        help="let check disable signing when no signing credentials are configured")
    args = parser.parse_args()
    if args.allow_unsigned and args.action != "check":
        parser.error("--allow-unsigned is only supported by check")
    signal.signal(signal.SIGTERM, interrupted)
    signal.signal(signal.SIGINT, interrupted)
    try:
        if args.action == "check":
            report_config(check_config(allow_unsigned=args.allow_unsigned))
        elif args.action == "cleanup":
            cleanup()
        elif args.archive is None or args.installer is None:
            parser.error("sign requires the ZIP and PKG paths")
        else:
            sign_package(args.archive, args.installer)
    except (RuntimeError, OSError, ValueError, KeyError) as error:
        print(f"::error::{error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
