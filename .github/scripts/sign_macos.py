#!/usr/bin/env python3
"""Sign and notarize macOS ZIP and PKG outputs for trusted CI builds."""

import argparse
import base64
import binascii
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


def run(*args, check=True, env=None):
    # Never echo commands: security/notarytool arguments contain credentials.
    result = subprocess.run([str(arg) for arg in args], text=True, capture_output=True, env=env)
    if check and result.returncode:
        if args[0] in ("codesign", "ditto", "spctl", "productbuild", "pkgutil") or args[:2] == ("xcrun", "stapler"):
            print(result.stderr, file=sys.stderr)
        raise RuntimeError(f"{Path(args[0]).name} failed (exit {result.returncode}).")
    return result


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
                run("security", "list-keychains", "-d", "user", "-s", *originals)
            except RuntimeError as error:
                errors.append(str(error))
        keychain = root / "signing.keychain-db"
        if keychain.exists():
            try:
                run("security", "delete-keychain", keychain)
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


def notarize_submission(submission, target, root, kind):
    diagnostics = Path(os.environ["RUNNER_TEMP"]) / "die-apple-notarization" / kind
    diagnostics.mkdir(parents=True, exist_ok=True)
    auth = notary_auth(root)
    print(f"Submitting signed {kind} to Apple notarization...", flush=True)
    result = run("xcrun", "notarytool", "submit", submission, *auth,
                 "--wait", "--timeout", "30m", "--output-format", "json", check=False)
    try:
        response = json.loads(result.stdout)
        if not isinstance(response, dict):
            raise ValueError
    except ValueError:
        raise RuntimeError("notarytool did not return a JSON result; "
                           "check the notarization credentials and network access.") from None
    (diagnostics / "submission.json").write_text(json.dumps(response, indent=2))
    submission_id = response.get("id", "")
    valid_id = isinstance(submission_id, str) and re.fullmatch(
        r"[0-9a-fA-F]{8}(?:-[0-9a-fA-F]{4}){3}-[0-9a-fA-F]{12}", submission_id)
    if valid_id:
        # The log is useful for accepted submissions with warnings, too.
        log = run("xcrun", "notarytool", "log", submission_id, *auth,
                  diagnostics / "notarization-log.json", check=False)
        if log.returncode:
            print("::warning::Apple's notarization log was not available.")
    if result.returncode or not valid_id or response.get("status") != "Accepted":
        raise RuntimeError("Apple did not accept this submission. "
                           "See the notarization diagnostic artifact.")
    run("xcrun", "stapler", "staple", target)
    run("xcrun", "stapler", "validate", target)


def notarize(app, root):
    submission = root / "submission.zip"
    run("ditto", "-c", "-k", "--sequesterRsrc", "--keepParent", app, submission)
    notarize_submission(submission, app, root, "app")
    run("codesign", "--verify", "--deep", "--strict", "--verbose=2", app)
    run("spctl", "--assess", "--type", "execute", "--verbose=2", app)


def notarize_installer(installer, root):
    notarize_submission(installer, installer, root, "installer")
    run("pkgutil", "--check-signature", installer)
    run("spctl", "--assess", "--type", "install", "--verbose=2", installer)


def require_output(path):
    if not path.is_file() or not path.stat().st_size:
        raise RuntimeError(f"Package tool did not create a nonempty output: {path.name}")


def sign_package(archive, installer):
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
    try:
        temporary_zip.unlink(missing_ok=True)
        temporary_pkg.unlink(missing_ok=True)
        keychain, identity, installer_identity = import_identities(root)
        extracted = root / "unpacked"
        run("ditto", "-x", "-k", archive, extracted)
        package = extracted / "die_mac_portable"
        app = app_in(package)
        sign_app(app, root, keychain, identity)
        notarize(app, root)
        run("ditto", "-c", "-k", "--sequesterRsrc", "--keepParent", package, temporary_zip)
        require_output(temporary_zip)
        # Validate the actual redistributable bytes after repacking the ticket.
        verified = root / "verified"
        run("ditto", "-x", "-k", temporary_zip, verified)
        final_app = app_in(verified / "die_mac_portable")
        run("codesign", "--verify", "--deep", "--strict", "--verbose=2", final_app)
        run("xcrun", "stapler", "validate", final_app)
        run("spctl", "--assess", "--type", "execute", "--verbose=2", final_app)
        # Run the signed CLI without Qt paths from the build environment or
        # signing credentials; this checks loading under the hardened runtime.
        runtime_env = {name: value for name, value in os.environ.items()
                       if not name.startswith(("APPLE_", "DYLD_", "QT_", "QML"))}
        run(final_app / "Contents/MacOS/diec", "--version", env=runtime_env)
        # Rebuild from the signed and stapled app: signing the original PKG
        # would leave its application payload unsigned.
        run("productbuild", "--component", app, "/Applications", "--sign",
            installer_identity, "--keychain", keychain, "--timestamp", temporary_pkg)
        require_output(temporary_pkg)
        run("pkgutil", "--check-signature", temporary_pkg)
        notarize_installer(temporary_pkg, root)
        # Replace unsigned outputs only after both pass every check.
        temporary_zip.replace(archive)
        temporary_pkg.replace(installer)
        print("Signed, notarized, and stapled ZIP and PKG verified.", flush=True)
    finally:
        temporary_zip.unlink(missing_ok=True)
        temporary_pkg.unlink(missing_ok=True)
        cleanup()


def interrupted(signum, frame):
    raise RuntimeError("Signing was interrupted; removing temporary credentials.")


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
