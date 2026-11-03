#!/usr/bin/env python3
"""Offline integration tests for the standalone Qt die-update executable.

Uses only Python's standard library, an ephemeral loopback HTTP server, and
temporary databases. No CDIE checkout, downloaded fixtures, or internet access
is needed. Copyright (c) 2026 hors<horsicq@gmail.com>. SPDX-License-Identifier: MIT
"""

import argparse
import hashlib
import io
import json
import os
from pathlib import Path
import signal
import subprocess
import sys
import tempfile
import threading
import unittest
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from urllib.parse import urlsplit
import zipfile


UPDATER = None
OK, AVAILABLE, NETWORK, DATABASE, ARGUMENT, VERIFY, CANCELLED = range(7)
STAMP = "die-update.ini"
OWNER, REPO, TAG = "fixture-owner", "fixture-repository", "current-database"


def release_tree(version):
    """Small valid signature scripts; the updater does not execute them."""
    script = ('function detect() { _setResult("tool", "Fixture", "%d.0", ""); '
              'return true; }\n' % version).encode()
    return {
        "db/PE/tool_fixture.sg": script,
        "db/release.txt": ("release %d\n" % version).encode(),
        "db/Empty/": b"",
        "db_extra/published-extra.sg": script,
        "db_custom/published-custom.sg": script,
    }


def make_zip(tree):
    output = io.BytesIO()
    with zipfile.ZipFile(output, "w") as archive:
        for name, contents in tree.items():
            entry = zipfile.ZipInfo(name, date_time=(2026, 10, 1, 12, 0, 0))
            entry.external_attr = ((0o40755 if name.endswith("/") else 0o100644) << 16)
            entry.compress_type = zipfile.ZIP_DEFLATED if name.endswith(".sg") else zipfile.ZIP_STORED
            archive.writestr(entry, contents)
    return output.getvalue()


def checksum(data):
    return (hashlib.md5(data).hexdigest() + "  db.zip\n").encode()


def snapshot(folder, include_stamp=True):
    """Include directory names and bytes so swaps cannot silently merge files."""
    folder = Path(folder)
    return {
        path.relative_to(folder).as_posix(): None if path.is_dir() else path.read_bytes()
        for path in folder.rglob("*")
        if include_stamp or path.relative_to(folder).as_posix() != STAMP
    }


class FixtureServer:
    """Serve registered files plus GitHub-shaped release metadata and redirects."""

    def __init__(self):
        self.routes = {}
        self.paused_responses = {}
        self.requests = []
        self.lock = threading.Lock()
        owner = self

        class Handler(BaseHTTPRequestHandler):
            def log_message(self, *_args):
                pass

            def answer(self, head=False):
                path = urlsplit(self.path).path
                with owner.lock:
                    owner.requests.append(path)
                    status, headers, body = owner.routes.get(path, (404, {}, b"Not found"))
                    pause = owner.paused_responses.get(path)
                self.send_response(status)
                for name, value in headers.items():
                    self.send_header(name, value)
                self.send_header("Content-Length", str(len(body)))
                self.end_headers()
                if not head:
                    if pause:
                        started, release = pause
                        started.set()
                        release.wait(10)
                    try:
                        self.wfile.write(body)
                    except (BrokenPipeError, ConnectionResetError):
                        pass  # Expected when a cancellation test disconnects.

            def do_GET(self):
                self.answer()

            def do_HEAD(self):
                self.answer(head=True)

        self.httpd = ThreadingHTTPServer(("127.0.0.1", 0), Handler)
        self.httpd.daemon_threads = True
        self.base = "http://127.0.0.1:%d" % self.httpd.server_address[1]
        self.api_base = self.base + "/api"
        self.release_path = "/%s/%s/releases/tag/%s" % (OWNER, REPO, TAG)
        self.release_url = self.base + self.release_path
        self.api_path = "/api/repos/%s/%s/releases/tags/%s" % (OWNER, REPO, TAG)
        self.thread = threading.Thread(target=self.httpd.serve_forever, daemon=True)
        self.thread.start()

    def close(self):
        self.httpd.shutdown()
        self.httpd.server_close()
        self.thread.join(5)

    def put(self, path, data, status=200, headers=None):
        with self.lock:
            self.routes[path] = (status, headers or {}, data)
        return self.base + path

    def plain(self, data, path="/files/db.zip", md5=True):
        url = self.put(path, data)
        if md5:
            self.put(path + ".md5", checksum(data))
        return url

    def release(self, data, version=1):
        assets = []
        for index, (name, body) in enumerate((("db.zip", data), ("db.zip.md5", checksum(data)))):
            download = "/%s/%s/releases/download/%s/%s" % (OWNER, REPO, TAG, name)
            cdn = "/cdn/%d/%s" % (version, name)
            self.put(cdn, body)
            self.put(download, b"", 302, {"Location": self.base + cdn})
            assets.append({
                "id": version * 10 + index,
                "name": name,
                "size": len(body),
                "state": "uploaded",
                "updated_at": "2026-10-%02dT12:00:00Z" % version,
                "browser_download_url": self.base + download,
            })
        metadata = {
            "id": version, "tag_name": TAG, "name": "Fixture release",
            "html_url": self.release_url, "draft": False, "prerelease": False,
            "published_at": "2026-10-01T12:00:00Z", "assets": assets,
        }
        self.put(self.api_path, json.dumps(metadata).encode(), headers={"Content-Type": "application/json"})
        return self.release_url


class UpdaterTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory(prefix="die-qt-updater-")
        self.addCleanup(temporary.cleanup)
        self.root = Path(temporary.name)
        self.db = self.root / "installation with spaces" / "db"
        self.server = FixtureServer()
        self.addCleanup(self.server.close)
        self.env = {
            name: value for name, value in os.environ.items()
            if not name.upper().startswith("CDIE_")
            and name.upper() not in ("HTTP_PROXY", "HTTPS_PROXY", "ALL_PROXY", "NO_PROXY")
        }
        self.env.update(CDIE_USER_DATA_DIR=str(self.root / "user-data"),
                        NO_PROXY="127.0.0.1,localhost", no_proxy="127.0.0.1,localhost")

    def command_line(self, *arguments, settings=False):
        args = [str(UPDATER), "--noproxy", "--nouserfolder", "--timeout", "3000"]
        if not settings:
            args.append("--nosettings")
        args.extend(str(argument) for argument in arguments)
        return args

    def command(self, *arguments, settings=False):
        return subprocess.run(self.command_line(*arguments, settings=settings),
                              cwd=self.root, env=self.env, capture_output=True,
                              text=True, encoding="utf-8", timeout=15)

    def update(self, *arguments, expect=OK, settings=False):
        result = self.command("--json", *arguments, settings=settings)
        context = "exit=%s\nstdout=%s\nstderr=%s" % (result.returncode, result.stdout, result.stderr)
        self.assertEqual(result.returncode, expect, context)
        try:
            report = json.loads(result.stdout)
        except ValueError:
            self.fail("Expected a JSON report: " + context)
        self.assertIsInstance(report, dict)
        return report

    def target(self, url):
        return ["-D", self.db, "--url", url]

    def install(self, version=1):
        tree = release_tree(version)
        data = make_zip(tree)
        url = self.server.plain(data)
        self.update(*self.target(url))
        self.assert_installed(tree)
        return url, data, tree

    def assert_installed(self, tree):
        expected = {name[3:].rstrip("/"): None if name.endswith("/") else data
                    for name, data in tree.items() if name.startswith("db/")}
        for name in list(expected):
            parent = Path(name).parent
            while parent != Path("."):
                expected[parent.as_posix()] = None
                parent = parent.parent
        self.assertEqual(snapshot(self.db, include_stamp=False), expected)
        self.assertTrue((self.db / STAMP).is_file())
        self.assert_no_staging()

    def assert_no_staging(self):
        self.assertEqual(list(self.db.parent.glob(".die-update*")), [])

    def reject_archive(self, data, before):
        url = self.server.plain(data)
        report = self.update(*self.target(url), expect=VERIFY)
        self.assertFalse(report["installed"])
        self.assertEqual(snapshot(self.db), before)
        self.assert_no_staging()

    def test_direct_check_default_update_and_check(self):
        data = make_zip(release_tree(1))
        url = self.server.plain(data)
        checked = self.update("--check", *self.target(url), expect=AVAILABLE)
        self.assertTrue(checked["updateAvailable"])
        self.assertFalse(self.db.exists())
        self.assertNotIn("/files/db.zip", self.server.requests)
        installed = self.update(*self.target(url))  # No --update means update.
        self.assertTrue(installed["installed"])
        self.assertEqual(installed["installedMd5"], hashlib.md5(data).hexdigest())
        self.assert_installed(release_tree(1))
        checked = self.update("--check", *self.target(url))
        self.assertFalse(checked["updateAvailable"])

    def test_github_release_redirect_and_json_contract(self):
        data = make_zip(release_tree(1))
        url = self.server.release(data)
        report = self.update(*self.target(url), "--api-base", self.server.api_base)
        fields = {
            "operation": str, "status": str, "updateAvailable": bool, "installed": bool,
            "userFolder": bool, "database": str, "source": str, "release": (str, type(None)),
            "archive": (str, type(None)), "archiveSize": (str, type(None)),
            "archiveTime": (str, type(None)), "remoteMd5": (str, type(None)),
            "installedMd5": (str, type(None)), "installedAt": (str, type(None)),
            "signatures": int, "message": str,
        }
        self.assertEqual(set(report), set(fields))
        for name, kind in fields.items():
            self.assertIsInstance(report[name], kind, name)
        self.assertEqual(report["release"], TAG)
        self.assertEqual(report["archiveSize"], str(len(data)))
        self.assertGreater(report["signatures"], 0)
        self.assertIn(self.server.api_path, self.server.requests)
        self.assertIn("/cdn/1/db.zip", self.server.requests)
        self.assert_installed(release_tree(1))

    def test_new_release_swaps_database_and_preserves_custom_and_extra(self):
        url, _, _ = self.install()
        for name in ("db_custom", "db_extra"):
            folder = self.db.parent / name
            folder.mkdir()
            (folder / "mine.sg").write_bytes(b"user-owned content\n")
        kept = {name: snapshot(self.db.parent / name) for name in ("db_custom", "db_extra")}
        (self.db / "obsolete.sg").write_text("obsolete release member\n")
        self.server.plain(make_zip(release_tree(2)))
        self.update("--check", *self.target(url), expect=AVAILABLE)
        self.update("--update", *self.target(url))
        self.assert_installed(release_tree(2))
        for name, contents in kept.items():
            self.assertEqual(snapshot(self.db.parent / name), contents)

    def test_current_update_leaves_files_unchanged(self):
        url, _, _ = self.install()
        before = {path.relative_to(self.db).as_posix(): (path.stat().st_mtime_ns, path.read_bytes())
                  for path in self.db.rglob("*") if path.is_file()}
        self.server.requests.clear()
        report = self.update(*self.target(url))
        self.assertFalse(report["installed"])
        after = {path.relative_to(self.db).as_posix(): (path.stat().st_mtime_ns, path.read_bytes())
                 for path in self.db.rglob("*") if path.is_file()}
        self.assertEqual(after, before)
        self.assertNotIn("/files/db.zip", self.server.requests)

    def test_force_replaces_a_current_database(self):
        url, _, tree = self.install()
        (self.db / "stray.sg").write_text("local modification\n")
        report = self.update("--force", *self.target(url))
        self.assertTrue(report["installed"])
        self.assert_installed(tree)

    def test_md5_mismatch_preserves_old_database(self):
        url, _, _ = self.install()
        before = snapshot(self.db)
        self.server.plain(make_zip(release_tree(2)))
        self.server.put("/files/db.zip.md5", checksum(b"different bytes"))
        report = self.update(*self.target(url), expect=VERIFY)
        self.assertIn("MD5", report["message"].upper())
        self.assertEqual(snapshot(self.db), before)
        self.assert_no_staging()

    def test_invalid_crc_preserves_old_database(self):
        self.install()
        before = snapshot(self.db)
        damaged = bytearray(make_zip(release_tree(2)))
        damaged[damaged.index(b"release 2\n")] ^= 0x20
        self.reject_archive(bytes(damaged), before)

    @unittest.skipIf(os.name == "nt", "POSIX SIGTERM cancellation semantics")
    def test_cancelling_a_download_preserves_old_database(self):
        url, _, _ = self.install()
        before = snapshot(self.db)
        self.server.plain(make_zip(release_tree(2)))
        started, release = threading.Event(), threading.Event()
        self.server.paused_responses["/files/db.zip"] = (started, release)
        process = subprocess.Popen(self.command_line("--json", *self.target(url)),
                                   cwd=self.root, env=self.env, stdout=subprocess.PIPE,
                                   stderr=subprocess.PIPE, text=True, encoding="utf-8")
        try:
            self.assertTrue(started.wait(5), "Updater did not request the ZIP")
            process.send_signal(signal.SIGTERM)
            stdout, stderr = process.communicate(timeout=10)
            self.assertEqual(process.returncode, CANCELLED, (stdout, stderr))
            report = json.loads(stdout)
            self.assertEqual(report["status"], "cancelled")
            self.assertFalse(report["installed"])
        finally:
            release.set()
            if process.poll() is None:
                process.kill()
                process.communicate(timeout=5)
        self.assertEqual(snapshot(self.db), before)
        self.assert_no_staging()

    def test_inactive_download_times_out_and_preserves_old_database(self):
        url, _, _ = self.install()
        before = snapshot(self.db)
        self.server.plain(make_zip(release_tree(2)))
        started, release = threading.Event(), threading.Event()
        self.server.paused_responses["/files/db.zip"] = (started, release)
        process = subprocess.Popen(self.command_line("--json", "--timeout", "200", *self.target(url)),
                                   cwd=self.root, env=self.env, stdout=subprocess.PIPE,
                                   stderr=subprocess.PIPE, text=True, encoding="utf-8")
        try:
            self.assertTrue(started.wait(5), "Updater did not request the ZIP")
            stdout, stderr = process.communicate(timeout=5)
            self.assertEqual(process.returncode, NETWORK, (stdout, stderr))
            report = json.loads(stdout)
            self.assertEqual(report["status"], "network-error")
            self.assertFalse(report["installed"])
        finally:
            release.set()
            if process.poll() is None:
                process.kill()
                process.communicate(timeout=5)
        self.assertEqual(snapshot(self.db), before)
        self.assert_no_staging()

    def test_traversal_preserves_old_database_and_cannot_escape(self):
        self.install()
        before = snapshot(self.db)
        for member in ("db/../../escaped.sg", "db\\..\\..\\escaped.sg"):
            with self.subTest(member=member):
                tree = release_tree(2)
                tree[member] = b"must never be extracted\n"
                self.reject_archive(make_zip(tree), before)
                self.assertEqual(list(self.root.rglob("escaped.sg")), [])

    def test_database_without_signatures_preserves_old_database(self):
        self.install()
        before = snapshot(self.db)
        self.reject_archive(make_zip({"db/README.txt": b"not a signature database\n"}), before)

    def test_invalid_and_truncated_zip_preserve_old_database(self):
        self.install()
        before = snapshot(self.db)
        good = make_zip(release_tree(2))
        for data in (b"this is not a zip", good[:len(good) // 2]):
            with self.subTest(size=len(data)):
                self.reject_archive(data, before)

    def test_explicit_settings_select_database_and_source(self):
        url = self.server.plain(make_zip(release_tree(1)))
        ini = self.root / "settings.ini"
        ini.write_text("[Scan]\nDie\\Database\\Main\\Path=%s\n"
                       "Die\\Database\\Main\\UpdateUrl=%s\n" % (self.db.as_posix(), url))
        original = ini.read_bytes()
        self.update("--settings", ini, settings=True)
        self.assert_installed(release_tree(1))
        self.assertEqual(ini.read_bytes(), original)

    def test_command_line_overrides_settings(self):
        ignored = self.root / "ignored" / "db"
        ignored_url = self.server.plain(make_zip(release_tree(1)), path="/ignored/db.zip")
        selected_url = self.server.plain(make_zip(release_tree(2)), path="/selected/db.zip")
        ini = self.root / "settings.ini"
        ini.write_text("[Scan]\nDie\\Database\\Main\\Path=%s\n"
                       "Die\\Database\\Main\\UpdateUrl=%s\n" % (ignored.as_posix(), ignored_url))
        self.update("--settings", ini, *self.target(selected_url), settings=True)
        self.assert_installed(release_tree(2))
        self.assertFalse(ignored.exists())
        self.assertFalse(any(path.startswith("/ignored/") for path in self.server.requests))

    def test_missing_settings_is_an_argument_error_without_network(self):
        result = self.command("--settings", self.root / "missing.ini", "--check",
                              *self.target(self.server.base + "/files/db.zip"), settings=True)
        self.assertEqual(result.returncode, ARGUMENT, result)
        self.assertIn("Settings file not found", result.stderr)
        self.assertEqual(self.server.requests, [])

    def test_invalid_cli_combinations_fail_before_network(self):
        for args in (("--check", "--force"), ("--check", "--update"), ("--unknown-option",),
                     ("--extradatabase", str(self.root / "extra")), ("-D",), ("--url",)):
            with self.subTest(args=args):
                result = self.command(*self.target(self.server.base + "/files/db.zip"), *args)
                self.assertEqual(result.returncode, ARGUMENT, result)
        self.assertEqual(self.server.requests, [])

    def test_api_rate_limit_falls_back_to_direct_release_downloads(self):
        data = make_zip(release_tree(1))
        url = self.server.release(data)
        body = json.dumps({"message": "API rate limit exceeded"}).encode()
        self.server.put(self.server.api_path, body, 403,
                        {"Content-Type": "application/json", "X-RateLimit-Remaining": "0"})
        self.update("--check", *self.target(url), "--api-base", self.server.api_base, expect=AVAILABLE)
        self.update(*self.target(url), "--api-base", self.server.api_base)
        self.assert_installed(release_tree(1))
        self.assertIn(self.server.api_path, self.server.requests)
        self.assertIn("/%s/%s/releases/download/%s/db.zip" % (OWNER, REPO, TAG), self.server.requests)

    def test_direct_zip_without_md5_is_compared_by_download(self):
        tree = release_tree(1)
        url = self.server.plain(make_zip(tree), path="/no-checksum/db.zip", md5=False)
        self.update(*self.target(url))
        self.assert_installed(tree)
        before = snapshot(self.db)
        self.server.requests.clear()
        # Check never downloads the archive; without metadata it cannot prove
        # that the installed copy matches. Update compares the actual bytes.
        self.update("--check", *self.target(url), expect=AVAILABLE)
        self.assertNotIn("/no-checksum/db.zip", self.server.requests)
        report = self.update(*self.target(url))
        self.assertFalse(report["updateAvailable"])
        self.assertFalse(report["installed"])
        self.assertIn("/no-checksum/db.zip", self.server.requests)
        self.assertEqual(snapshot(self.db), before)

    def test_url_credentials_are_not_printed_or_persisted(self):
        url = self.server.plain(make_zip(release_tree(1)))
        credential_url = url.replace("http://", "http://fixture-user:private-fixture-password@", 1)
        result = self.command("--json", *self.target(credential_url))
        self.assertEqual(result.returncode, OK, result)
        self.assert_installed(release_tree(1))
        text = result.stdout + result.stderr + (self.db / STAMP).read_text()
        self.assertNotIn("private-fixture-password", text)
        self.assertNotIn("fixture-user", text)
        checked = self.command("--check", *self.target(credential_url))
        self.assertEqual(checked.returncode, OK, checked)
        self.assertNotIn("private-fixture-password", checked.stdout + checked.stderr)

    def test_help_version_and_quiet_update(self):
        for option in ("--help", "--version"):
            result = self.command(option)
            self.assertEqual(result.returncode, OK, result)
            self.assertTrue(result.stdout.strip())
        self.assertEqual(self.server.requests, [])
        url = self.server.plain(make_zip(release_tree(1)))
        result = self.command("--quiet", *self.target(url))
        self.assertEqual((result.returncode, result.stdout, result.stderr), (OK, "", ""))
        self.assert_installed(release_tree(1))


def main():
    global UPDATER
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--updater", required=True, type=Path, help="path to the built die-update executable")
    parser.add_argument("tests", nargs="*", help="optional unittest test names")
    args = parser.parse_args()
    UPDATER = args.updater.resolve()
    if not UPDATER.is_file():
        parser.error("Updater executable does not exist: %s" % UPDATER)
    loader = unittest.defaultTestLoader
    suite = (loader.loadTestsFromNames(args.tests, sys.modules[__name__]) if args.tests
             else loader.loadTestsFromTestCase(UpdaterTests))
    return not unittest.TextTestRunner(verbosity=2).run(suite).wasSuccessful()


if __name__ == "__main__":
    raise SystemExit(main())
