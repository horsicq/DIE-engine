"""Release table previews and body-only updates, with GitHub calls mocked."""

import contextlib
import importlib.util
import io
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch


SCRIPTS = Path(__file__).resolve().parents[1]


def load(name):
    spec = importlib.util.spec_from_file_location(name, SCRIPTS / (name + ".py"))
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


renderer = load("release_notes")
with patch.dict(sys.modules, {"release_notes": renderer}):
    updater = load("update_release_notes")


def asset(name):
    return {"name": name, "size": 123, "state": "uploaded",
            "browser_download_url": "https://github.com/horsicq/DIE-engine/releases/download/4.0.0/" + name,
            "url": "https://api.github.com/repos/horsicq/DIE-engine/releases/assets/123"}


class UpdateTests(unittest.TestCase):
    def setUp(self):
        self.release = {"id": 42, "tag_name": "4.0.0", "body": "Old description",
                        "html_url": "https://github.com/horsicq/DIE-engine/releases/tag/4.0.0"}
        self.assets = [asset("die_4.0.0_mac_arm64_qt6.dmg"), asset("die_sourcecode_4.0.0.tar.gz")]
        self.pages = [[self.assets[0]], [self.assets[1]]]
        self.calls = []
        self.stdout, self.stderr = io.StringIO(), io.StringIO()
        stack = contextlib.ExitStack()
        self.addCleanup(stack.close)
        stack.enter_context(contextlib.redirect_stdout(self.stdout))
        stack.enter_context(contextlib.redirect_stderr(self.stderr))
        stack.enter_context(patch.object(updater, "github_api", side_effect=self.api))

    def api(self, endpoint, *, payload=None, paginate=False):
        self.calls.append((endpoint, payload, paginate))
        if payload is not None:
            return dict(self.release, body=payload["body"])
        return self.pages if paginate else self.release

    def test_preview_uses_all_asset_pages_and_download_links_without_writing(self):
        updater.update_release("horsicq/DIE-engine")
        self.assertEqual(self.calls, [
            ("repos/horsicq/DIE-engine/releases/latest", None, False),
            ("repos/horsicq/DIE-engine/releases/42/assets?per_page=100", None, True),
        ])
        text = self.stdout.getvalue()
        self.assertIn("| Asset / File | Description / Host OS |", text)
        self.assertIn("[CHANGELOG]", text)
        self.assertIn("Disk image for macOS (ARM64, Qt 6)", text)
        self.assertLess(text.index("Source code tarball"), text.index("Disk image"))
        self.assertIn(self.assets[0]["browser_download_url"], text)
        self.assertNotIn("api.github.com", text)
        self.assertIn("--apply", self.stderr.getvalue())

    def test_apply_addresses_exact_release_id_and_only_changes_body(self):
        updater.update_release("horsicq/DIE-engine", apply=True)
        self.assertEqual(self.calls[-1], ("repos/horsicq/DIE-engine/releases/42",
                                         {"body": renderer.render_release_notes(self.assets)}, False))
        self.assertIn("Updated horsicq/DIE-engine 4.0.0 (2 assets)", self.stdout.getvalue())

    def test_tag_is_encoded_and_preview_can_be_saved(self):
        with tempfile.TemporaryDirectory() as directory:
            output = Path(directory) / "notes.md"
            updater.update_release("horsicq/DIE-engine", "v4/rc1", output=output)
            self.assertEqual(output.read_text(), renderer.render_release_notes(self.assets))
        self.assertEqual(self.calls[0][0], "repos/horsicq/DIE-engine/releases/tags/v4%2Frc1")
        self.assertEqual(self.stdout.getvalue(), "")
        self.assertEqual(len(self.calls), 2)

    def test_identical_body_is_not_updated_again(self):
        self.release["body"] = renderer.render_release_notes(self.assets).strip()
        updater.update_release("horsicq/DIE-engine", apply=True)
        self.assertEqual(len(self.calls), 2)
        self.assertIn("already up to date", self.stdout.getvalue())

    def test_empty_incomplete_or_zero_byte_assets_block_update(self):
        for pages in ([[]], [[dict(self.assets[0], state="starter")]], [[dict(self.assets[0], size=0)]]):
            with self.subTest(pages=pages):
                self.pages = pages
                self.calls.clear()
                with self.assertRaises(ValueError):
                    updater.update_release("horsicq/DIE-engine", apply=True)
                self.assertTrue(all(payload is None for _, payload, _ in self.calls))

    def test_read_failure_stops_before_update(self):
        with patch.object(updater, "github_api", side_effect=RuntimeError("API unavailable")) as api:
            with self.assertRaisesRegex(RuntimeError, "API unavailable"):
                updater.update_release("horsicq/DIE-engine", apply=True)
        self.assertEqual(api.call_count, 1)

    def test_invalid_repository_never_reaches_api(self):
        with self.assertRaises(ValueError):
            updater.update_release("https://github.com/horsicq/DIE-engine", apply=True)
        self.assertEqual(self.calls, [])


class ApiTests(unittest.TestCase):
    def test_body_is_json_on_stdin_and_never_shell_interpreted(self):
        payload = {"body": "First line\n`literal` $(literal) \"quotes\""}
        result = subprocess.CompletedProcess([], 0, '{"id":42}', "")
        with patch.object(updater.subprocess, "run", return_value=result) as run:
            self.assertEqual(updater.github_api("repos/owner/repo/releases/42", payload=payload), {"id": 42})
        self.assertEqual(run.call_args.args[0], ["gh", "api", "--hostname", "github.com",
                                               "repos/owner/repo/releases/42", "--method", "PATCH", "--input", "-"])
        self.assertEqual(json.loads(run.call_args.kwargs["input"]), payload)
        self.assertNotIn("shell", run.call_args.kwargs)

    def test_missing_cli_and_api_failure_are_reported(self):
        with patch.object(updater.subprocess, "run", side_effect=FileNotFoundError):
            with self.assertRaisesRegex(RuntimeError, "GitHub CLI is required"):
                updater.github_api("repos/owner/repo/releases/latest")
        with patch.object(updater.subprocess, "run", return_value=subprocess.CompletedProcess([], 1, "", "")):
            with self.assertRaisesRegex(RuntimeError, "GitHub API request failed"):
                updater.github_api("repos/owner/repo/releases/latest")


class RenderingTests(unittest.TestCase):
    def test_unknown_platform_does_not_break_markdown_table(self):
        notes = renderer.render_release_notes([asset("die_4.0.0_Foo|Bar_amd64.deb")])
        self.assertIn("Installer for Foo\\|Bar", notes)
        self.assertIn("die_4.0.0_Foo\\|Bar_amd64.deb", notes)


if __name__ == "__main__":
    unittest.main()
