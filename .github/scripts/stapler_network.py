#!/usr/bin/env python3
"""Temporarily use TCP for Apple ticket retrieval on hosted macOS runners.

Work around https://github.com/actions/runner-images/issues/14924 without
replacing the main PF ruleset or disabling another service's packet filter.
"""

import argparse
import json
import os
from pathlib import Path
import platform
import re
import secrets
import shutil
import subprocess
import sys


COMMAND_TIMEOUT = 20
ANCHOR_PREFIX = "com.apple/die-stapler-"


def state_dir():
    return Path(os.environ["RUNNER_TEMP"]) / "die-stapler-network"


def require_hosted_macos():
    if (sys.platform != "darwin" or platform.machine() not in ("x86_64", "arm64")
            or os.environ.get("GITHUB_ACTIONS") != "true"
            or os.environ.get("RUNNER_ENVIRONMENT") != "github-hosted"):
        raise RuntimeError("The stapler network workaround is only for GitHub-hosted Intel or ARM64 macOS runners.")


def pfctl(*args, output_file=None):
    command = ["sudo", "-n", "/sbin/pfctl", *args]
    print(f"PF operation: {args[0]}", flush=True)
    try:
        if output_file is None:
            result = subprocess.run(command, text=True, capture_output=True, timeout=COMMAND_TIMEOUT)
            output = result.stdout + result.stderr
        else:
            # Persist the enable token even if the parent is interrupted after
            # PF is enabled. The always-run cleanup step can recover it.
            with output_file.open("w", encoding="utf-8") as output:
                result = subprocess.run(command, text=True, stdout=output,
                                        stderr=subprocess.STDOUT, timeout=COMMAND_TIMEOUT)
            output = output_file.read_text(encoding="utf-8")
    except subprocess.TimeoutExpired:
        raise RuntimeError(f"pfctl {args[0]} timed out after {COMMAND_TIMEOUT} seconds.") from None
    if result.returncode:
        raise RuntimeError(f"pfctl {args[0]} failed (exit {result.returncode}): {output.strip()}")
    return output


def save_state(root, state):
    temporary = root / "state.tmp"
    temporary.write_text(json.dumps(state), encoding="utf-8")
    temporary.replace(root / "state.json")


def enable():
    require_hosted_macos()
    root = state_dir()
    root.mkdir(mode=0o700)
    state = {"anchor": ANCHOR_PREFIX + secrets.token_hex(8), "enable_started": False}
    save_state(root, state)
    rules = pfctl("-s", "rules")
    if not re.search(r'^\s*anchor\s+"com\.apple/\*"(?:\s|$)', rules, re.MULTILINE):
        raise RuntimeError("The active PF ruleset has no com.apple/* anchor; refusing to replace the main ruleset.")
    rule_file = root / "rules.conf"
    rule_file.write_text("block return out quick proto udp from any to any port 443\n", encoding="ascii")
    pfctl("-a", state["anchor"], "-f", str(rule_file))
    state["enable_started"] = True
    save_state(root, state)
    output = pfctl("-E", output_file=root / "enable-output.txt")
    if not re.search(r'^\s*Token\s*:\s*([0-9]+)\s*$', output, re.MULTILINE):
        raise RuntimeError("pfctl did not return an enable token; cleanup will report the unreleased reference.")
    print(f"Apple ticket retrieval will use HTTPS over TCP on this {platform.machine()} macOS runner.", flush=True)


def cleanup():
    root = state_dir()
    if not root.exists():
        return
    require_hosted_macos()
    state_file = root / "state.json"
    if not state_file.exists():
        # No mutation is made until the state has been written.
        shutil.rmtree(root)
        return
    state = json.loads(state_file.read_text(encoding="utf-8"))
    anchor = state.get("anchor", "")
    if not re.fullmatch(re.escape(ANCHOR_PREFIX) + r"[0-9a-f]{16}", anchor):
        raise RuntimeError("Invalid stapler PF anchor in cleanup state; refusing to change unrelated rules.")
    errors = []
    try:
        pfctl("-a", anchor, "-F", "rules")
    except RuntimeError as error:
        errors.append(str(error))
    if state.get("enable_started"):
        output_file = root / "enable-output.txt"
        output = output_file.read_text(encoding="utf-8") if output_file.exists() else ""
        token = re.search(r'^\s*Token\s*:\s*([0-9]+)\s*$', output, re.MULTILINE)
        if token is None:
            errors.append("PF enable token was not recorded; cannot safely release the reference.")
        else:
            try:
                pfctl("-X", token.group(1))
                state["enable_started"] = False
                save_state(root, state)
            except RuntimeError as error:
                errors.append(str(error))
    if errors:
        raise RuntimeError("; ".join(errors))
    shutil.rmtree(root)
    print("Removed the temporary stapler PF anchor and released its enable reference.", flush=True)


def main():
    os.umask(0o077)
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("action", choices=("enable", "cleanup"))
    args = parser.parse_args()
    try:
        enable() if args.action == "enable" else cleanup()
    except (RuntimeError, OSError, ValueError, KeyError) as error:
        print(f"::error::{error}", file=sys.stderr, flush=True)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
