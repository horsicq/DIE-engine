#!/usr/bin/env python3
"""Regression check for scanning files reached through directory symlinks.

Usage:
    python3 src/console/tests/test_symlink_scan.py --diec <build>/src/console/diec

The test copies diec itself as the scan target, so it needs no fixture corpus
and no signature database. It exits successfully when the platform cannot
create symbolic links (for example an unprivileged Windows shell).
"""

import argparse
import os
import shutil
import subprocess
import sys
import tempfile


def run_diec(diec, arguments):
    return subprocess.run([diec] + arguments, capture_output=True, text=True, timeout=120)


def assert_info_output(label, result):
    if result.returncode != 0:
        raise AssertionError(f"{label}: diec exited with {result.returncode}\n{result.stderr}")
    for marker in ("File name:", "File type:"):
        if marker not in result.stdout:
            raise AssertionError(f"{label}: missing '{marker}' in output\nSTDOUT:\n{result.stdout}\nSTDERR:\n{result.stderr}")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--diec", required=True, help="path to the built diec executable")
    options = parser.parse_args()

    diec = os.path.abspath(options.diec)
    if not os.path.isfile(diec):
        parser.error(f"diec not found: {diec}")

    with tempfile.TemporaryDirectory(prefix="diec-symlink-") as temp_dir:
        real_dir = os.path.join(temp_dir, "test_real")
        sub_dir = os.path.join(real_dir, "sub")
        os.makedirs(sub_dir)

        target = os.path.join(real_dir, "target.bin")
        second_target = os.path.join(sub_dir, "second.bin")
        shutil.copyfile(diec, target)
        shutil.copyfile(diec, second_target)

        link_dir = os.path.join(temp_dir, "test_symb")
        try:
            os.symlink(real_dir, link_dir, target_is_directory=True)
        except (OSError, NotImplementedError) as error:
            print(f"SKIP: cannot create a directory symlink: {error}")
            return 0

        direct_result = run_diec(diec, ["-i", target])
        assert_info_output("direct file", direct_result)

        linked_file = os.path.join(link_dir, "target.bin")
        linked_file_result = run_diec(diec, ["-i", linked_file])
        assert_info_output("file through directory symlink", linked_file_result)

        linked_dir_result = run_diec(diec, ["-i", link_dir])
        assert_info_output("directory symlink", linked_dir_result)

        loop_link = os.path.join(real_dir, "loop")
        recursion_arguments = ["-i", "-r", link_dir]
        try:
            os.symlink(real_dir, loop_link, target_is_directory=True)
        except (OSError, NotImplementedError) as error:
            print(f"SKIP recursive cycle check: cannot create a loop symlink: {error}")
        else:
            recursive_result = run_diec(diec, recursion_arguments)
            assert_info_output("recursive directory symlink", recursive_result)
            for marker in (target, second_target):
                if os.path.basename(marker) not in recursive_result.stdout:
                    raise AssertionError(f"recursive scan did not report {os.path.basename(marker)}\n{recursive_result.stdout}")

        print("OK")
        return 0


if __name__ == "__main__":
    sys.exit(main())
