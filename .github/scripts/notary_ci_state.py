#!/usr/bin/env python3
"""Restore and publish minimal notarization state across attempts of one CI run."""

import argparse
from datetime import datetime
import io
import json
import os
from pathlib import Path
import re
import shutil
import stat
import subprocess
import sys
import tempfile
import time
import zipfile


MAX_ARCHIVE_BYTES = 8 * 1024 * 1024
MAX_JSON_BYTES = 32 * 1024
MAX_MEMBERS = 1000
MAX_PAGES = 20
API_TIMEOUT = 45
RESTORE_TIMEOUT = 240
HASH = re.compile(r"[0-9a-f]{64}")
UUID = re.compile(r"[0-9a-fA-F]{8}(?:-[0-9a-fA-F]{4}){3}-[0-9a-fA-F]{12}")
# Keep old PKG records for recovery history, but isolate them from DMG IDs.
KINDS = ("app", "dmg", "installer")
STATE_MEMBER = re.compile(r"state/(" + "|".join(KINDS) + r")/([0-9a-f]{64})\.json")
_deadline = None


def context():
    value = {
        "schema_version": 1,
        "repository": os.environ["GITHUB_REPOSITORY"],
        "run_id": os.environ["GITHUB_RUN_ID"],
        "run_attempt": int(os.environ["GITHUB_RUN_ATTEMPT"]),
        "head_sha": os.environ["GITHUB_SHA"].lower(),
        "arch": os.environ["CMAKE_OSX_ARCHITECTURES"],
        "qt_major": os.environ["QT_MAJOR"],
    }
    if (not re.fullmatch(r"[A-Za-z0-9_.-]+/[A-Za-z0-9_.-]+", value["repository"])
            or not re.fullmatch(r"[1-9][0-9]*", value["run_id"])
            or value["run_attempt"] < 1
            or not re.fullmatch(r"[0-9a-f]{40}", value["head_sha"])
            or value["arch"] not in ("x86_64", "arm64")
            or value["qt_major"] not in ("5", "6")):
        raise RuntimeError("Invalid CI context for notarization state.")
    return value


def state_name(value, attempt=None):
    return (f"die-macos-notary-state-{value['arch']}-qt{value['qt_major']}"
            f"-attempt-{value['run_attempt'] if attempt is None else attempt}")


def parse_json(data):
    if len(data) > MAX_JSON_BYTES:
        raise RuntimeError("Notarization state JSON exceeds its size limit.")
    try:
        value = json.loads(data)
    except (ValueError, TypeError):
        raise RuntimeError("Invalid notarization state JSON.") from None
    if not isinstance(value, dict):
        raise RuntimeError("Notarization state JSON must be an object.")
    return value


def normalize_state(value, expected_hash=None):
    if not isinstance(value, dict):
        raise RuntimeError("Notarization state must be an object.")
    digest = value.get("sha256")
    if not isinstance(digest, str) or HASH.fullmatch(digest) is None or (
            expected_hash is not None and digest != expected_hash):
        raise RuntimeError("Notarization state has an invalid or mismatched file hash.")
    result = {"sha256": digest}
    if "id" in value:
        if not isinstance(value["id"], str) or UUID.fullmatch(value["id"]) is None:
            raise RuntimeError("Notarization state has an invalid submission ID.")
        result["id"] = value["id"].lower()
    upload_state = value.get("upload_state")
    if upload_state not in ("submitted", "uncertain") or ("id" not in result and upload_state != "uncertain"):
        raise RuntimeError("Notarization state has an invalid upload outcome.")
    result["upload_state"] = upload_state
    for field in ("upload_started_at", "status_checked_at"):
        if field in value:
            text = value[field]
            try:
                valid = isinstance(text, str) and len(text) <= 64 and datetime.fromisoformat(
                    text.replace("Z", "+00:00")).tzinfo is not None
            except ValueError:
                valid = False
            if not valid:
                raise RuntimeError("Notarization state has an invalid timestamp.")
            result[field] = text
    if "upload_returncode" in value:
        code = value["upload_returncode"]
        if type(code) is not int or not -(2 ** 31) <= code < 2 ** 31:
            raise RuntimeError("Notarization state has an invalid upload result.")
        result["upload_returncode"] = code
    if "upload_timed_out" in value:
        if type(value["upload_timed_out"]) is not bool:
            raise RuntimeError("Notarization state has an invalid timeout result.")
        result["upload_timed_out"] = value["upload_timed_out"]
    if "status" in value:
        if "id" not in result or value["status"] not in ("In Progress", "Accepted", "Invalid", "Rejected"):
            raise RuntimeError("Notarization state has an invalid status.")
        result["status"] = value["status"]
    return result


def merge(records, kind, value):
    key = (kind, value["sha256"])
    previous = records.get(key)
    if previous:
        old_id, new_id = previous.get("id"), value.get("id")
        if old_id and new_id and old_id != new_id:
            raise RuntimeError("Conflicting notarization IDs exist for the same file hash.")
        if old_id and not new_id:
            return
    records[key] = value


def api_bytes(endpoint, limit):
    remaining = API_TIMEOUT if _deadline is None else min(API_TIMEOUT, _deadline - time.monotonic())
    if remaining <= 0:
        raise RuntimeError("Timed out restoring notarization state from GitHub.")
    # gh handles authenticated API redirects without forwarding the token to
    # artifact storage. Never echo its arguments, environment, or raw stderr.
    with tempfile.TemporaryFile() as output:
        try:
            result = subprocess.run(
                ["gh", "api", "--hostname", "github.com", "--allow-escape-sequences", endpoint],
                stdout=output, stderr=subprocess.PIPE, timeout=remaining, check=False,
            )
        except (subprocess.TimeoutExpired, OSError):
            raise RuntimeError("Could not retrieve saved notarization state from GitHub; refusing an empty restore.") from None
        if result.returncode:
            raise RuntimeError("GitHub rejected notarization state retrieval; refusing an empty restore.")
        if output.tell() > limit:
            raise RuntimeError("GitHub notarization state response exceeds its size limit.")
        output.seek(0)
        return output.read()


def prior_artifacts(value):
    prefix = state_name(value, "")
    pattern = re.compile(re.escape(prefix) + r"([1-9][0-9]*)")
    found = {}
    for page in range(1, MAX_PAGES + 1):
        payload = api_bytes(
            f"repos/{value['repository']}/actions/runs/{value['run_id']}/artifacts?per_page=100&page={page}",
            MAX_ARCHIVE_BYTES,
        )
        try:
            entries = json.loads(payload)["artifacts"]
        except (ValueError, TypeError, KeyError):
            raise RuntimeError("Invalid GitHub artifact listing.") from None
        if not isinstance(entries, list) or len(entries) > 100:
            raise RuntimeError("Invalid GitHub artifact listing.")
        for artifact in entries:
            if not isinstance(artifact, dict) or not isinstance(artifact.get("name"), str):
                raise RuntimeError("Invalid GitHub artifact metadata.")
            name = artifact["name"]
            if not name.startswith(prefix):
                continue
            match = pattern.fullmatch(name)
            if match is None:
                raise RuntimeError("Invalid notarization state artifact name.")
            attempt = int(match.group(1))
            if attempt >= value["run_attempt"]:
                continue
            workflow = artifact.get("workflow_run", {})
            if (not isinstance(workflow, dict) or str(workflow.get("id")) != value["run_id"]
                    or workflow.get("head_sha") != value["head_sha"]):
                raise RuntimeError("Notarization state artifact belongs to a different workflow run or commit.")
            if artifact.get("expired") is not False:
                raise RuntimeError("Saved notarization state has expired; refusing to silently submit again.")
            size, identifier = artifact.get("size_in_bytes"), artifact.get("id")
            if type(identifier) is not int or identifier < 1 or type(size) is not int or not 0 < size <= MAX_ARCHIVE_BYTES:
                raise RuntimeError("Invalid notarization state artifact size or ID.")
            if attempt in found and found[attempt]["id"] != identifier:
                raise RuntimeError("Multiple notarization state artifacts exist for one attempt.")
            found[attempt] = artifact
        if len(entries) < 100:
            return sorted(found.items())
    raise RuntimeError("Too many artifact pages to safely restore all notarization state.")


def read_archive(data, value, attempt):
    if len(data) > MAX_ARCHIVE_BYTES:
        raise RuntimeError("Notarization state archive exceeds its size limit.")
    expected_manifest = dict(value, run_attempt=attempt)
    records = {}
    try:
        with zipfile.ZipFile(io.BytesIO(data)) as archive:
            entries = archive.infolist()
            if len(entries) > MAX_MEMBERS or sum(item.file_size for item in entries) > MAX_ARCHIVE_BYTES:
                raise RuntimeError("Notarization state archive exceeds its expanded size limit.")
            seen = set()
            for entry in entries:
                name = entry.filename
                if name in seen or entry.flag_bits & 1 or stat.S_ISLNK(entry.external_attr >> 16):
                    raise RuntimeError("Unsafe or duplicate member in notarization state archive.")
                seen.add(name)
                if entry.is_dir() and (name == "state/" or name in (f"state/{kind}/" for kind in KINDS)):
                    continue
                match = STATE_MEMBER.fullmatch(name)
                if name != "manifest.json" and match is None:
                    raise RuntimeError("Unexpected member in notarization state archive.")
                if entry.file_size > MAX_JSON_BYTES:
                    raise RuntimeError("Notarization state JSON exceeds its size limit.")
                item = parse_json(archive.read(entry))
                if name == "manifest.json":
                    if (type(item.get("schema_version")) is not int or type(item.get("run_attempt")) is not int
                            or any(item.get(key) != expected for key, expected in expected_manifest.items())):
                        raise RuntimeError("Notarization state manifest does not match this run and variant.")
                else:
                    merge(records, match.group(1), normalize_state(item, match.group(2)))
            if "manifest.json" not in seen:
                raise RuntimeError("Notarization state archive has no manifest.")
    except (zipfile.BadZipFile, NotImplementedError, RuntimeError) as error:
        if isinstance(error, RuntimeError) and not isinstance(error, zipfile.BadZipFile):
            raise
        raise RuntimeError("Could not read the notarization state archive.") from None
    return records


def read_restored():
    root = Path(os.environ["RUNNER_TEMP"]) / "die-apple-notary-restored"
    records = {}
    if not root.exists() and not root.is_symlink():
        return records
    if root.is_symlink():
        raise RuntimeError("Refusing a symlinked notarization restore directory.")
    for kind in KINDS:
        if (root / kind).is_symlink():
            raise RuntimeError("Refusing a symlinked notarization restore directory.")
        for path in sorted((root / kind).glob("*.json")):
            if path.is_symlink() or HASH.fullmatch(path.stem) is None:
                raise RuntimeError("Invalid restored notarization state path.")
            merge(records, kind, normalize_state(parse_json(path.read_bytes()), path.stem))
    return records


def publish_directory(destination, records, manifest=None):
    if destination.exists() or destination.is_symlink():
        raise RuntimeError("Notarization state destination already exists; refusing to overwrite it.")
    staging = Path(tempfile.mkdtemp(prefix="die-notary-state-", dir=destination.parent))
    try:
        if manifest is not None:
            (staging / "manifest.json").write_text(json.dumps(manifest, indent=2))
        for (kind, digest), value in records.items():
            target = staging / ("state" if manifest is not None else "") / kind / f"{digest}.json"
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_text(json.dumps(value, indent=2))
        staging.replace(destination)
    finally:
        if staging.exists():
            shutil.rmtree(staging)


def restore():
    global _deadline
    value = context()
    records = {}
    _deadline = time.monotonic() + RESTORE_TIMEOUT
    try:
        if value["run_attempt"] > 1:
            artifacts = prior_artifacts(value)
            if not artifacts:
                raise RuntimeError("No prior notarization state artifact was found for this rerun; refusing to silently submit again.")
            for attempt, artifact in artifacts:
                print(f"Restoring notarization state from attempt {attempt}.", flush=True)
                archive = api_bytes(f"repos/{value['repository']}/actions/artifacts/{artifact['id']}/zip", MAX_ARCHIVE_BYTES)
                for (kind, _), state in read_archive(archive, value, attempt).items():
                    merge(records, kind, state)
        publish_directory(Path(os.environ["RUNNER_TEMP"]) / "die-apple-notary-restored", records)
    finally:
        _deadline = None
    print(f"Restored {len(records)} notarization state record(s) for this run and variant.", flush=True)


def save():
    value = context()
    records = read_restored()
    diagnostics = Path(os.environ["RUNNER_TEMP"]) / "die-apple-notarization"
    for kind in KINDS:
        path = diagnostics / kind / "submission.json"
        if path.exists() or path.is_symlink():
            if path.is_symlink():
                raise RuntimeError("Refusing a symlinked notarization state file.")
            merge(records, kind, normalize_state(parse_json(path.read_bytes())))
    publish_directory(Path(os.environ["RUNNER_TEMP"]) / "die-apple-notary-state", records, value)
    print(f"Prepared {len(records)} notarization state record(s) for CI rerun recovery.", flush=True)


def main():
    os.umask(0o077)
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("action", choices=("restore", "save"))
    args = parser.parse_args()
    try:
        restore() if args.action == "restore" else save()
    except (RuntimeError, OSError, ValueError, KeyError) as error:
        print(f"::error::{error}", file=sys.stderr, flush=True)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
