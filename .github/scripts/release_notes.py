#!/usr/bin/env python3
"""Render the release asset table in the format used by the 3.21 release."""

import argparse
import html
import json
from pathlib import Path
import re
from urllib.parse import quote


CHANGELOG = "https://github.com/horsicq/Detect-It-Easy/blob/master/changelog.txt"
RUN_GUIDE = "https://github.com/horsicq/Detect-It-Easy/blob/master/docs/RUN.md"
ARCHITECTURES = {
    "amd64": "x86_64",
    "x86_64": "x86_64",
    "aarch64": "ARM64",
    "arm64": "ARM64",
    "ppc64el": "PowerPC 64-bit little-endian",
    "riscv64": "RISC-V 64-bit",
    "s390x": "s390x",
    "i386": "x86",
    "x86": "x86",
    "x64": "x64",
}


def describe_asset(name):
    """Return display order and a description inferred from the package name."""
    lower = name.lower()
    arch_match = re.search(
        r"(?:^|[-_])(x86_64|amd64|aarch64|arm64|ppc64el|riscv64|s390x|i386|x86|x64)(?=[._-]|$)",
        lower,
    )
    arch = ARCHITECTURES[arch_match.group(1)] if arch_match else ""
    suffix = f" ({arch})" if arch else ""

    if lower.startswith("die_sourcecode_"):
        return 0, "Source code tarball"
    if lower.endswith(".appimage"):
        return 10, f"Portable version for Linux{suffix} [How to run]({RUN_GUIDE})"
    if lower.endswith(".deb"):
        match = re.fullmatch(r"die_[0-9.]+_(.+)_([a-z0-9]+)\.deb", name, re.IGNORECASE)
        if match:
            platform = match.group(1)
            host = {
                "jammy": "Ubuntu 22.04",
                "noble": "Ubuntu 24.04",
                "kalilinux": "Kali Linux",
                "parrotos": "Parrot OS",
                "linux": "Linux (DEB)",
            }.get(platform.lower(), table_text(platform.replace("_", " ")))
            return 20, f"Installer for {host}{suffix}"
        return 20, f"Installer for Linux (DEB){suffix}"
    if lower.endswith(".tar.gz") and "_portable_ubuntu_" in lower:
        match = re.search(r"_ubuntu_([0-9.]+)_", lower)
        version = f" {match.group(1)}" if match else ""
        return 25, f"Portable version for Ubuntu{version}{suffix}"
    if lower.endswith((".pkg.tar.zst", ".pkg.tar.xz")):
        return 30, f"Installer for Arch Linux{suffix}"
    if lower.endswith(".rpm"):
        if "fedora" in lower:
            match = re.search(r"_fedora_([0-9]+)_", lower)
            version = f" {match.group(1)}" if match else ""
            return 40, f"Installer for Fedora{version}{suffix}"
        if "opensuse" in lower:
            return 45, f"Installer for openSUSE{suffix}"
        return 49, f"Installer for Linux (RPM){suffix}"
    if lower.endswith(".nar.xz"):
        return 50, "Package closure for Nix"
    if lower.startswith("detect-it-easy-") and lower.endswith(".pkg"):
        return 60, "Installer for FreeBSD"
    if "termux" in lower:
        return 70, f"Command-line version for Termux on Android{suffix}"
    if lower.startswith("die_mac") or "_mac_portable" in lower or re.fullmatch(
        r"die_[0-9.]+_mac_(?:x86_64|arm64)_qt[56]\.(?:dmg|pkg)", lower
    ):
        kind = "Disk image" if lower.endswith(".dmg") else (
            "Installer" if lower.endswith(".pkg") else "Portable version"
        )
        qt = "Qt 6" if "qt6" in lower else "Qt 5" if "qt5" in lower else ""
        details = ", ".join(value for value in (arch, qt) if value)
        details = f" ({details})" if details else ""
        return 80, f"{kind} for macOS{details}"
    if lower.startswith("die_win"):
        if lower.startswith("die_winxp"):
            return 95, "Portable version for Windows XP (x86)"
        arch = "ARM64" if lower.startswith("die_winarm64") else (
            "x86" if lower.startswith("die_win32") else "x64"
        )
        qt = ", Qt 6" if "qt6" in lower else ""
        return 90, f"Portable version for Windows ({arch}{qt})"
    return 100, "Release asset"


def table_text(value):
    text = html.escape(value, quote=False).replace("\n", " ").replace("\r", " ")
    return re.sub(r"([\\\[\]|])", r"\\\1", text)


def render_release_notes(assets):
    if not assets:
        raise ValueError("Cannot generate release notes without uploaded assets")
    rows = []
    for asset in assets:
        name = asset["name"]
        # gh release view calls the browser download URL 'url'; REST uses the longer name.
        url = asset.get("browser_download_url") or asset["url"]
        if not url.startswith("https://"):
            raise ValueError(f"Expected an HTTPS download URL for {name}")
        order, description = describe_asset(name)
        link = f"[{table_text(name)}]({quote(url, safe=':/%?=&+#')})"
        rows.append((order, name.casefold(), f"| {link} | {description} |"))

    lines = [
        f"Please see the file [CHANGELOG]({CHANGELOG}) for a detailed list of changes.",
        "",
        "| Asset / File | Description / Host OS |",
        "| ----------- | ----------- |",
        *(row for _, _, row in sorted(rows)),
        "",
        "DIE library: https://github.com/horsicq/die_library",
        "Python binding: https://github.com/elastic/die-python",
        "",
    ]
    return "\n".join(lines)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--release-json", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    args = parser.parse_args()
    release = json.loads(args.release_json.read_text(encoding="utf-8"))
    args.output.write_text(render_release_notes(release["assets"]), encoding="utf-8")


if __name__ == "__main__":
    main()
