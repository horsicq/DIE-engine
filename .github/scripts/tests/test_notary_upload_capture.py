"""Capture notarization IDs before upload failure, using fake executables only."""

import contextlib
import hashlib
import importlib.util
import io
import json
import os
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch


SCRIPT = Path(__file__).resolve().parents[1] / "sign_macos.py"
SPEC = importlib.util.spec_from_file_location("sign_macos_upload_capture", SCRIPT)
sign_macos = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(sign_macos)
SUBMISSION_ID = "11111111-2222-3333-4444-555555555555"
OTHER_ID = "11111111-2222-3333-4444-666666666666"


class SubmissionOutputTests(unittest.TestCase):
    def test_json_and_normal_output_identify_only_submission_ids(self):
        outputs = (
            json.dumps({"id": SUBMISSION_ID, "message": "Successfully uploaded file"}),
            f"Submission ID received\n  id: {SUBMISSION_ID}\nUpload progress: 5%\n",
            f"Successfully uploaded file.\n  id: {SUBMISSION_ID}\n",
            f"Submission ID: {SUBMISSION_ID}\n",
            f"Submission ID received\n  id: {SUBMISSION_ID.upper()}\n"
            f"Successfully uploaded file\n  id: {SUBMISSION_ID}\n",
        )
        for output in outputs:
            with self.subTest(output=output):
                self.assertEqual(sign_macos.submission_id_from_output(output), SUBMISSION_ID)

    def test_stderr_submission_marker_is_recovered(self):
        self.assertEqual(sign_macos.submission_id_from_output(
            "", f"Submission ID received\n  id: {SUBMISSION_ID}\n"), SUBMISSION_ID)

    def test_unrelated_or_incomplete_ids_are_not_used(self):
        for output in (f"Request ID: {SUBMISSION_ID}", f"  id: {SUBMISSION_ID}",
                       json.dumps({"error": {"id": SUBMISSION_ID}}),
                       "Submission ID received\n  id: not-a-uuid\n",
                       "Submission ID received\n  id: 11111111-2222-3333-4444-5555"):
            with self.subTest(output=output):
                self.assertIsNone(sign_macos.submission_id_from_output(output))

    def test_conflicting_submission_ids_are_rejected(self):
        with self.assertRaises(RuntimeError):
            sign_macos.submission_id_from_output(
                f"Submission ID received\n  id: {SUBMISSION_ID}\n",
                f"Submission ID: {OTHER_ID}\n")

    def test_diagnostics_remove_credentials_urls_headers_and_pem(self):
        text = (
            "private-notary-password\n"
            "Error: connection failed https://storage.example.invalid/object?X-Amz-Signature=signed-token\n"
            "http://username:url-password@example.invalid/private\n"
            "Authorization: Bearer server-generated-token\n"
            "X-Amz-Security-Token: server-session-token\n"
            "-----BEGIN PRIVATE KEY-----\nnot-an-environment-value\n-----END PRIVATE KEY-----\n"
        )
        with patch.dict(os.environ, {"APPLE_NOTARY_PASSWORD": "private-notary-password"}):
            result = sign_macos.sanitize_notary_output(text)
        self.assertIn("connection failed", result)
        for secret in ("private-notary-password", "storage.example.invalid", "signed-token",
                       "url-password", "server-generated-token", "server-session-token",
                       "not-an-environment-value"):
            self.assertNotIn(secret, result)

    def test_diagnostics_are_bounded(self):
        self.assertLessEqual(len(sign_macos.sanitize_notary_output("x" * 100000)), 16 * 1024)

    def test_json_credentials_escaped_urls_and_incomplete_private_keys_are_removed(self):
        text = json.dumps({"Authorization": "Bearer temporary-token",
                           "X-Amz-Security-Token": "temporary-session",
                           "url": "https://storage.example.invalid/file?signature=temporary-signature"}).replace("/", "\\/")
        text += "\n-----BEGIN PRIVATE KEY-----\npartial-private-key"
        result = sign_macos.sanitize_notary_output(text)
        for secret in ("temporary-token", "temporary-session", "temporary-signature", "partial-private-key"):
            self.assertNotIn(secret, result)


class UploadCaptureTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory(prefix="die-notary-upload-capture-")
        self.addCleanup(temporary.cleanup)
        self.root = Path(temporary.name)
        self.bin = self.root / "bin"
        self.bin.mkdir()
        self.submission = self.root / "submission.zip"
        self.submission.write_bytes(b"signed app bytes")
        self.diagnostics = self.root / "die-apple-notarization/app"
        self.diagnostics.mkdir(parents=True)
        self.state_file = self.diagnostics / "submission.json"
        self.invocations = self.root / "invocations"
        self.observed = self.root / "observed-saved-id"
        self.output = io.StringIO()
        self.stack = contextlib.ExitStack()
        self.addCleanup(self.stack.close)
        self.stack.enter_context(patch.dict(os.environ, {
            "RUNNER_TEMP": str(self.root),
            "PATH": str(self.bin) + os.pathsep + os.defpath,
            "APPLE_NOTARY_PASSWORD": "private-notary-password",
        }, clear=True))
        self.stack.enter_context(patch.object(sign_macos, "_signing_deadline", None))
        self.stack.enter_context(patch.object(sign_macos, "NOTARY_OUTPUT_INTERVAL", 0.02))
        self.stack.enter_context(contextlib.redirect_stdout(self.output))
        self.stack.enter_context(contextlib.redirect_stderr(self.output))

    def executable(self, body):
        script = self.bin / "xcrun"
        script.write_text(f"#!{sys.executable}\n" + body)
        script.chmod(0o700)

    def prepare(self):
        return sign_macos.prepare_notary_submission(self.submission, [], self.diagnostics, "app")

    def state(self):
        return json.loads(self.state_file.read_text())

    def test_id_is_saved_while_upload_still_runs_and_failed_upload_is_not_repeated(self):
        self.executable(
            "import json, pathlib, sys, time\n"
            f"state_file = pathlib.Path({str(self.state_file)!r})\n"
            f"invocations = pathlib.Path({str(self.invocations)!r})\n"
            "with invocations.open('a') as output: output.write('submit\\n')\n"
            "assert '--no-s3-acceleration' in sys.argv\n"
            "assert '--no-wait' in sys.argv\n"
            "assert sys.argv[sys.argv.index('--output-format') + 1] == 'normal'\n"
            f"print('Submission ID received\\n  id: {SUBMISSION_ID}', flush=True)\n"
            "deadline = time.monotonic() + 3\n"
            "while time.monotonic() < deadline:\n"
            f"    if json.loads(state_file.read_text()).get('id') == {SUBMISSION_ID!r}:\n"
            f"        pathlib.Path({str(self.observed)!r}).touch()\n"
            "        break\n"
            "    time.sleep(0.01)\n"
            "print('Error: connection failed private-notary-password', file=sys.stderr, flush=True)\n"
            "sys.exit(1)\n")
        first = self.prepare()
        self.assertTrue(self.observed.exists(), "Submission ID was not saved until the child exited")
        self.assertEqual(first["id"], SUBMISSION_ID)
        self.assertEqual(first["upload_state"], "uncertain")
        self.assertEqual(first["upload_returncode"], 1)
        self.assertEqual(first["sha256"], hashlib.sha256(self.submission.read_bytes()).hexdigest())
        second = self.prepare()
        self.assertEqual(second["id"], SUBMISSION_ID)
        self.assertEqual(self.invocations.read_text(), "submit\n")
        saved_diagnostics = "".join(path.read_text() for path in self.diagnostics.rglob("*.json"))
        self.assertNotIn("private-notary-password", saved_diagnostics + self.output.getvalue())

    def test_timeout_preserves_partial_id_without_a_second_submission(self):
        self.executable(
            "import pathlib, time\n"
            f"with pathlib.Path({str(self.invocations)!r}).open('a') as output: output.write('submit\\n')\n"
            f"print('Submission ID received\\n  id: {SUBMISSION_ID}', flush=True)\n"
            "time.sleep(30)\n")
        original_run = sign_macos.run

        def short_run(*args, **kwargs):
            kwargs["timeout"] = 0.2
            return original_run(*args, **kwargs)

        with patch.object(sign_macos, "run", side_effect=short_run):
            first = self.prepare()
        self.assertEqual(first["id"], SUBMISSION_ID)
        self.assertTrue(first["upload_timed_out"])
        self.assertEqual(first["upload_state"], "uncertain")
        self.assertEqual(self.prepare()["id"], SUBMISSION_ID)
        self.assertEqual(self.invocations.read_text(), "submit\n")

    def test_failure_without_id_keeps_duplicate_upload_blocked(self):
        self.executable(
            "import pathlib, sys\n"
            f"with pathlib.Path({str(self.invocations)!r}).open('a') as output: output.write('submit\\n')\n"
            f"print('Error: request {OTHER_ID} failed', file=sys.stderr, flush=True)\n"
            "sys.exit(1)\n")
        for attempt in range(2):
            with self.subTest(attempt=attempt), self.assertRaises(RuntimeError):
                self.prepare()
        self.assertEqual(self.invocations.read_text(), "submit\n")
        self.assertEqual(self.state()["upload_state"], "uncertain")
        self.assertNotIn("id", self.state())

    def test_conflicting_later_id_invalidates_early_id_and_still_blocks_resubmission(self):
        self.executable(
            "import json, pathlib, sys, time\n"
            f"state_file = pathlib.Path({str(self.state_file)!r})\n"
            f"with pathlib.Path({str(self.invocations)!r}).open('a') as output: output.write('submit\\n')\n"
            f"print('Submission ID received\\n  id: {SUBMISSION_ID}', flush=True)\n"
            "deadline = time.monotonic() + 3\n"
            "while time.monotonic() < deadline:\n"
            f"    if json.loads(state_file.read_text()).get('id') == {SUBMISSION_ID!r}: break\n"
            "    time.sleep(0.01)\n"
            f"print('Successfully uploaded file\\n  id: {OTHER_ID}', flush=True)\n"
            "sys.exit(1)\n")
        with self.assertRaisesRegex(RuntimeError, "conflicting"):
            self.prepare()
        self.assertNotIn("id", self.state())
        with self.assertRaisesRegex(RuntimeError, "no usable submission ID"):
            self.prepare()
        self.assertEqual(self.invocations.read_text(), "submit\n")


if __name__ == "__main__":
    unittest.main()
