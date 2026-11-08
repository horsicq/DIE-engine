#!/usr/bin/env python3
"""Generate the latest release's download table; use --apply to update GitHub.

Requires Python 3 and GitHub CLI (gh auth login, or GH_TOKEN in CI).
Examples from the repository root:
  python3 .github/scripts/update_release_notes.py --output release-notes.md
  python3 .github/scripts/update_release_notes.py --apply
  python3 .github/scripts/update_release_notes.py --tag 4.0.0 --apply
"""

import argparse
import json
from pathlib import Path
import re
import subprocess
import sys
from urllib.parse import quote

from release_notes import render_release_notes


def github_api(endpoint, *, payload=None, paginate=False):
    command = ["gh", "api", "--hostname", "github.com", endpoint]
    if paginate:
        command += ["--paginate", "--slurp"]
    if payload is not None:
        command += ["--method", "PATCH", "--input", "-"]
    try:
        result = subprocess.run(command, input=json.dumps(payload) if payload is not None else None,
                                capture_output=True, text=True, encoding="utf-8", timeout=120)
    except FileNotFoundError:
        raise RuntimeError("GitHub CLI is required. Install gh and run gh auth login.") from None
    except subprocess.TimeoutExpired:
        raise RuntimeError("GitHub API request timed out.") from None
    if result.returncode:
        raise RuntimeError(f"GitHub API request failed (exit {result.returncode}); "
                           "check gh auth status and repository access.")
    try:
        return json.loads(result.stdout)
    except ValueError:
        raise RuntimeError("GitHub returned an invalid JSON response.") from None


def update_release(repo, tag=None, *, apply=False, output=None):
    if re.fullmatch(r"[A-Za-z0-9_.-]+/[A-Za-z0-9_.-]+", repo) is None:
        raise ValueError("Repository must use OWNER/REPO format.")
    selector = "latest" if tag is None else "tags/" + quote(tag, safe="")
    release = github_api(f"repos/{repo}/releases/{selector}")
    release_id = release.get("id")
    if type(release_id) is not int or release_id <= 0 or not release.get("tag_name"):
        raise ValueError("GitHub returned an invalid release.")
    endpoint = f"repos/{repo}/releases/{release_id}"
    # The separate paginated endpoint also covers releases with over 100 files.
    pages = github_api(endpoint + "/assets?per_page=100", paginate=True)
    if not isinstance(pages, list) or any(not isinstance(page, list) for page in pages):
        raise ValueError("GitHub returned an invalid asset list.")
    assets = [asset for page in pages for asset in page]
    if any(asset.get("state") != "uploaded" or asset.get("size", 0) <= 0 for asset in assets):
        raise ValueError("Some release assets are empty or still uploading; try again when uploads finish.")
    notes = render_release_notes(assets)
    label = f"{repo} {release['tag_name']} ({len(assets)} assets)"
    if output is not None:
        output.write_text(notes, encoding="utf-8")
        print(f"Saved table for {label} to {output}", file=sys.stderr)
    if not apply:
        if output is None:
            sys.stdout.write(notes)
        print(f"Preview for {label}. Use --apply to update the release description.", file=sys.stderr)
        return
    if (release.get("body") or "").strip() == notes.strip():
        print(f"Release table is already up to date: {release['html_url']}")
        return
    # Address the release by ID: a new 'latest' release cannot redirect this edit.
    # Send only the body, preserving tag, title, assets and publication settings.
    updated = github_api(endpoint, payload={"body": notes})
    if updated.get("id") != release_id or (updated.get("body") or "").strip() != notes.strip():
        raise RuntimeError("GitHub did not confirm the updated release description.")
    print(f"Updated {label}: {release['html_url']}")


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--repo", default="horsicq/DIE-engine", help="repository (default: horsicq/DIE-engine)")
    parser.add_argument("--tag", help="specific release tag; default is GitHub's latest published stable release")
    parser.add_argument("--output", type=Path, help="also save the generated Markdown to this file")
    parser.add_argument("--apply", action="store_true", help="update the release description on GitHub")
    args = parser.parse_args()
    try:
        update_release(args.repo, args.tag, apply=args.apply, output=args.output)
    except (RuntimeError, OSError, ValueError, KeyError, TypeError) as error:
        print(f"Error: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
