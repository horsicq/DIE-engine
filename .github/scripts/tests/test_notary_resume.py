"""Notarization recovery contracts with fake Apple responses and virtual time."""

import contextlib
import hashlib
import importlib.util
import io
import json
import os
from pathlib import Path
import subprocess
import tempfile
import unittest
from unittest.mock import patch


SCRIPT = Path(__file__).resolve().parents[1] / "sign_macos.py"
SPEC = importlib.util.spec_from_file_location("sign_macos_resume", SCRIPT)
sign_macos = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(sign_macos)
SUBMISSION_ID = "11111111-2222-3333-4444-555555555555"
OTHER_ID = "11111111-2222-3333-4444-666666666666"


class NotaryResumeTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory(prefix="die-notary-resume-")
        self.addCleanup(temporary.cleanup)
        self.root = Path(temporary.name)
        self.submission = self.root / "signed.zip"
        self.submission.write_bytes(b"signed app ZIP contents")
        self.target = self.root / "die.app"
        self.target.mkdir()
        self.diagnostics = self.root / "die-apple-notarization" / "app"
        self.state_file = self.diagnostics / "submission.json"
        self.now = 0.0
        self.calls = []
        self.sleeps = []
        self.info_events = []
        self.log_events = []
        self.submit_event = (0, {"id": SUBMISSION_ID, "message": "Successfully uploaded file"})
        self.info_default = "Accepted"
        self.output = io.StringIO()
        self.stack = contextlib.ExitStack()
        self.addCleanup(self.stack.close)
        self.stack.enter_context(patch.dict(os.environ, {
            "RUNNER_TEMP": str(self.root),
            "APPLE_NOTARY_APPLE_ID": "notary@example.invalid",
            "APPLE_NOTARY_PASSWORD": "private-notary-password",
            "APPLE_NOTARY_TEAM_ID": "TEAMID",
        }, clear=True))
        self.stack.enter_context(patch.object(sign_macos, "_signing_deadline", None))
        self.stack.enter_context(patch.object(sign_macos.time, "monotonic", side_effect=lambda: self.now))
        self.stack.enter_context(patch.object(sign_macos.time, "sleep", side_effect=self.sleep))
        self.stack.enter_context(patch.object(sign_macos, "run", side_effect=self.fake_run))
        self.stack.enter_context(contextlib.redirect_stdout(self.output))
        self.stack.enter_context(contextlib.redirect_stderr(self.output))

    def sleep(self, seconds):
        self.assertGreater(seconds, 0)
        self.sleeps.append(seconds)
        self.now += seconds

    def state(self):
        return json.loads(self.state_file.read_text())

    def commands(self, command):
        return [call for call in self.calls if call[0][:3] == ("xcrun", "notarytool", command)]

    def stapler_calls(self):
        return [call for call in self.calls if call[0][:2] == ("xcrun", "stapler")]

    def completed(self, args, event):
        if isinstance(event, BaseException):
            raise event
        if isinstance(event, str):
            event = (0, {"id": SUBMISSION_ID, "status": event})
        code, payload = event
        stdout = payload if isinstance(payload, str) else json.dumps(payload)
        return subprocess.CompletedProcess(args, code, stdout, "private-notary-password")

    def fake_run(self, *args, **kwargs):
        self.calls.append((args, kwargs))
        if args[:3] == ("xcrun", "notarytool", "submit"):
            self.assertIn("--no-wait", args)
            self.assertNotIn("--wait", args)
            self.assertEqual(kwargs["timeout"], 35 * 60)
            self.assertTrue(self.state_file.exists(), "Record uncertain upload before invoking Apple")
            self.assertEqual(self.state()["sha256"], hashlib.sha256(self.submission.read_bytes()).hexdigest())
            return self.completed(args, self.submit_event)
        if args[:3] == ("xcrun", "notarytool", "info"):
            self.assertEqual(args[3], SUBMISSION_ID)
            # This also catches implementations that persist only after polling.
            self.assertEqual(self.state()["id"], SUBMISSION_ID)
            self.assertEqual(self.state()["sha256"], hashlib.sha256(self.submission.read_bytes()).hexdigest())
            self.assertGreater(kwargs["timeout"], 0)
            self.assertLessEqual(kwargs["timeout"], 60)
            return self.completed(args, self.info_events.pop(0) if self.info_events else self.info_default)
        if args[:3] == ("xcrun", "notarytool", "log"):
            self.assertEqual(args[3], SUBMISSION_ID)
            self.assertGreater(kwargs["timeout"], 0)
            self.assertLessEqual(kwargs["timeout"], 60)
            path = Path(args[-1])
            self.assertEqual(path, self.diagnostics / "notarization-log.partial")
            event = self.log_events.pop(0) if self.log_events else (
                0, {"jobId": SUBMISSION_ID, "status": self.info_default, "issues": None})
            if isinstance(event, BaseException):
                path.write_text('{"incomplete":')
                raise event
            code, payload = event
            path.write_text(payload if isinstance(payload, str) else json.dumps(payload))
            return subprocess.CompletedProcess(args, code, "", "private-notary-password")
        return subprocess.CompletedProcess(args, 0, "", "")

    def notarize(self, kind="app"):
        return sign_macos.notarize_submission(self.submission, self.target, self.root, kind)

    def restored_state(self, state, *, kind="app", digest=None):
        digest = digest or hashlib.sha256(self.submission.read_bytes()).hexdigest()
        path = self.root / "die-apple-notary-restored" / kind / f"{digest}.json"
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(state if isinstance(state, str) else json.dumps(state))
        return path

    def accepted_state(self):
        return {"sha256": hashlib.sha256(self.submission.read_bytes()).hexdigest(),
                "id": SUBMISSION_ID, "upload_state": "submitted", "status": "Accepted"}

    def test_id_is_saved_before_polling_and_upload_is_never_repeated_on_connection_loss(self):
        self.info_events = [
            (1, "connection failed private-notary-password"),
            sign_macos.CommandTimeout("status request timed out"),
            (0, "malformed private-notary-password"),
            "In Progress", "Accepted",
        ]
        self.log_events = [
            sign_macos.CommandTimeout("log request timed out"),
            (1, '{"incomplete":'),
            (0, {"jobId": SUBMISSION_ID, "status": "Accepted", "issues": None}),
        ]
        self.notarize()
        self.assertEqual(len(self.commands("submit")), 1)
        self.assertEqual(len(self.commands("info")), 5)
        self.assertEqual(len(self.commands("log")), 3)
        self.assertEqual(self.state()["status"], "Accepted")
        self.assertEqual(self.state()["upload_state"], "submitted")
        self.assertEqual(len(self.stapler_calls()), 2)
        self.assertFalse((self.diagnostics / "notarization-log.partial").exists())
        combined = self.output.getvalue() + self.state_file.read_text()
        for secret in ("private-notary-password", "notary@example.invalid"):
            self.assertNotIn(secret, combined)

    def test_saved_id_resumes_identical_payload_without_another_upload(self):
        self.info_events = [(1, "connection lost")] * 5
        with self.assertRaises(RuntimeError):
            self.notarize()
        self.assertEqual(self.state()["id"], SUBMISSION_ID)
        self.assertEqual(len(self.commands("submit")), 1)
        self.assertFalse(self.stapler_calls())
        self.notarize()
        self.assertEqual(len(self.commands("submit")), 1)
        self.assertEqual(self.state()["status"], "Accepted")
        self.assertEqual(len(self.stapler_calls()), 2)

    def test_fresh_runner_restores_matching_id_without_uploading_again(self):
        self.info_events = [(1, "connection lost")] * 5
        with self.assertRaises(RuntimeError):
            self.notarize()
        previous_state = self.state()
        uploaded_bytes = self.submission.read_bytes()
        self.assertEqual(len(self.commands("submit")), 1)

        # A new attempt has a fresh RUNNER_TEMP and only the restored state,
        # never the previous attempt's active submission.json or credentials.
        self.root = self.root / "fresh-runner"
        self.root.mkdir()
        self.submission = self.root / "signed.zip"
        self.submission.write_bytes(uploaded_bytes)
        self.target = self.root / "die.app"
        self.target.mkdir()
        self.diagnostics = self.root / "die-apple-notarization" / "app"
        self.state_file = self.diagnostics / "submission.json"
        restored = self.restored_state(previous_state)
        self.assertFalse(self.state_file.exists())
        previous_calls = len(self.calls)
        with patch.dict(os.environ, {"RUNNER_TEMP": str(self.root)}):
            self.notarize()

        self.assertEqual(len(self.commands("submit")), 1)
        self.assertEqual(self.calls[previous_calls][0][:3], ("xcrun", "notarytool", "info"))
        self.assertEqual(self.state()["id"], previous_state["id"])
        self.assertEqual(self.state()["sha256"], previous_state["sha256"])
        self.assertEqual(self.state()["status"], "Accepted")
        self.assertEqual(json.loads(restored.read_text()), previous_state)
        self.assertEqual(len(self.stapler_calls()), 2)

    def test_restored_different_hash_allows_one_new_upload_for_new_bytes(self):
        state = self.accepted_state()
        restored = self.restored_state(state)
        self.submission.write_bytes(b"new signed bytes with different timestamp")
        self.notarize()
        self.assertEqual(len(self.commands("submit")), 1)
        self.assertNotEqual(self.state()["sha256"], state["sha256"])
        self.assertEqual(json.loads(restored.read_text()), state)

    def test_restored_app_and_installer_state_are_not_interchangeable(self):
        state = self.accepted_state()
        state["id"] = OTHER_ID
        self.restored_state(state, kind="installer")
        self.notarize()
        self.assertEqual(len(self.commands("submit")), 1)
        self.assertEqual(self.state()["id"], SUBMISSION_ID)

    def test_dmg_never_reuses_app_or_legacy_installer_id_for_same_hash(self):
        state = self.accepted_state()
        state["id"] = OTHER_ID
        self.restored_state(state, kind="app")
        self.restored_state(state, kind="installer")
        self.diagnostics = self.root / "die-apple-notarization/dmg"
        self.state_file = self.diagnostics / "submission.json"
        self.notarize("dmg")
        self.assertEqual(len(self.commands("submit")), 1)
        self.assertEqual(self.state()["id"], SUBMISSION_ID)

    def test_restored_dmg_matching_hash_reuses_id_without_upload(self):
        self.restored_state(self.accepted_state(), kind="dmg")
        self.diagnostics = self.root / "die-apple-notarization/dmg"
        self.state_file = self.diagnostics / "submission.json"
        self.notarize("dmg")
        self.assertFalse(self.commands("submit"))
        self.assertEqual(self.state()["id"], SUBMISSION_ID)
        self.assertEqual(len(self.commands("info")), 1)
        self.assertEqual(len(self.stapler_calls()), 2)

    def test_restored_ambiguous_upload_blocks_duplicate_after_ci_rerun(self):
        state = {"sha256": hashlib.sha256(self.submission.read_bytes()).hexdigest(),
                 "upload_state": "uncertain", "upload_timed_out": True}
        self.restored_state(state)
        for invocation in range(2):
            with self.subTest(invocation=invocation), self.assertRaises(RuntimeError):
                self.notarize()
        self.assertEqual(self.state()["sha256"], state["sha256"])
        self.assertEqual(self.state()["upload_state"], "uncertain")
        self.assertEqual(self.calls, [])

    def test_corrupt_matching_restored_record_never_becomes_a_cache_miss(self):
        cases = (
            {**self.accepted_state(), "sha256": "0" * 64},
            {**self.accepted_state(), "id": "not-a-uuid"},
            {"sha256": self.accepted_state()["sha256"], "upload_state": "submitted"},
            '["not a state object"]',
            '{"incomplete":',
        )
        for state in cases:
            with self.subTest(state=state):
                self.state_file.unlink(missing_ok=True)
                self.restored_state(state)
                with self.assertRaises(RuntimeError):
                    self.notarize()
                self.assertEqual(self.calls, [])

    def test_restored_acceptance_requires_a_fresh_matching_apple_status(self):
        self.restored_state(self.accepted_state())
        self.info_default = "Invalid"
        with self.assertRaisesRegex(RuntimeError, "Apple did not accept"):
            self.notarize()
        self.assertFalse(self.commands("submit"))
        self.assertEqual(len(self.commands("info")), 1)
        self.assertEqual(self.state()["status"], "Invalid")
        self.assertFalse(self.stapler_calls())

    def test_restored_state_promotes_only_allowlisted_fields(self):
        state = {**self.accepted_state(), "unexpected_secret": "must-not-be-uploaded-again",
                 "APPLE_NOTARY_PASSWORD": "private-notary-password"}
        self.restored_state(state)
        self.notarize()
        self.assertFalse(self.commands("submit"))
        self.assertNotIn("unexpected_secret", self.state())
        self.assertNotIn("APPLE_NOTARY_PASSWORD", self.state())
        self.assertNotIn("must-not-be-uploaded-again", self.state_file.read_text() + self.output.getvalue())

    def test_oversized_or_linked_restored_record_is_rejected_before_upload(self):
        restored = self.restored_state({**self.accepted_state(), "padding": "x" * (16 * 1024)})
        with self.assertRaises(RuntimeError):
            self.notarize()
        self.assertEqual(self.calls, [])
        self.state_file.unlink(missing_ok=True)
        other = self.root / "unexpected-state.json"
        other.write_text(json.dumps(self.accepted_state()))
        restored.unlink()
        restored.symlink_to(other)
        with self.assertRaises(RuntimeError):
            self.notarize()
        self.assertEqual(self.calls, [])
        other.unlink()
        self.assertFalse(restored.exists())
        self.assertTrue(restored.is_symlink())
        with self.assertRaises(RuntimeError):
            self.notarize()
        self.assertEqual(self.calls, [])

    def test_different_payload_cannot_reuse_saved_acceptance_or_upload_again(self):
        self.notarize()
        count = len(self.calls)
        self.submission.write_bytes(b"different signed bytes")
        with self.assertRaises(RuntimeError):
            self.notarize()
        self.assertEqual(len(self.calls), count)
        self.assertEqual(len(self.commands("submit")), 1)

    def test_ambiguous_upload_without_id_is_not_repeated_on_reentry(self):
        self.submit_event = sign_macos.CommandTimeout("upload connection lost")
        with self.assertRaises(RuntimeError):
            self.notarize()
        self.assertEqual(self.state()["upload_state"], "uncertain")
        self.assertNotIn("id", self.state())
        with self.assertRaises(RuntimeError):
            self.notarize()
        self.assertEqual(len(self.commands("submit")), 1)
        self.assertFalse(self.commands("info"))
        self.assertFalse(self.stapler_calls())

    def test_nonzero_upload_with_id_is_recovered_by_querying_that_id(self):
        self.submit_event = (1, {"id": SUBMISSION_ID})
        self.notarize()
        self.assertEqual(len(self.commands("submit")), 1)
        self.assertEqual(len(self.commands("info")), 1)
        self.assertEqual(self.state()["status"], "Accepted")

    def test_upload_response_never_replaces_a_fresh_status_check(self):
        self.submit_event = (0, {"id": SUBMISSION_ID, "status": "Accepted"})
        self.info_default = "Invalid"
        with self.assertRaisesRegex(RuntimeError, "Apple did not accept"):
            self.notarize()
        self.assertEqual(len(self.commands("info")), 1)
        self.assertEqual(self.state()["status"], "Invalid")
        self.assertFalse(self.stapler_calls())

    def test_upload_timeout_with_complete_json_id_is_recovered(self):
        self.submit_event = sign_macos.CommandTimeout("upload timed out", stdout=json.dumps({"id": SUBMISSION_ID}))
        self.notarize()
        self.assertEqual(len(self.commands("submit")), 1)
        self.assertEqual(len(self.commands("info")), 1)
        self.assertEqual(self.state()["status"], "Accepted")

    def test_mismatched_info_id_fails_immediately_without_stapling_or_reupload(self):
        self.info_events = [(0, {"id": OTHER_ID, "status": "Accepted"})]
        with self.assertRaises(RuntimeError):
            self.notarize()
        self.assertEqual(len(self.commands("submit")), 1)
        self.assertEqual(len(self.commands("info")), 1)
        self.assertFalse(self.stapler_calls())
        self.assertEqual(self.state()["id"], SUBMISSION_ID)

    def test_unknown_status_is_retried_and_cannot_authorize_stapling(self):
        self.info_events = ["Something Else"] * 5
        with self.assertRaises(RuntimeError):
            self.notarize()
        self.assertEqual(len(self.commands("submit")), 1)
        self.assertEqual(len(self.commands("info")), 5)
        self.assertFalse(self.stapler_calls())

    def test_invalid_saved_id_fails_before_running_any_apple_command(self):
        self.notarize()
        state = self.state()
        state["id"] = "not-a-valid-submission-id"
        self.state_file.write_text(json.dumps(state))
        count = len(self.calls)
        with self.assertRaises(RuntimeError):
            self.notarize()
        self.assertEqual(len(self.calls), count)

    def test_rejection_remains_fatal_after_log_connection_failures(self):
        self.info_default = "Invalid"
        self.log_events = [sign_macos.CommandTimeout("log request timed out")] * 3
        with self.assertRaisesRegex(RuntimeError, "Apple did not accept"):
            self.notarize()
        self.assertEqual(self.state()["status"], "Invalid")
        self.assertEqual(len(self.commands("log")), 3)
        self.assertFalse(self.stapler_calls())
        self.assertFalse((self.diagnostics / "notarization-log.partial").exists())

    def test_bad_log_id_or_partial_json_is_retried_before_atomic_publish(self):
        self.log_events = [
            (0, {"jobId": OTHER_ID, "status": "Accepted"}),
            (0, '{"incomplete":'),
            (0, {"jobId": SUBMISSION_ID, "status": "Accepted", "issues": None}),
        ]
        self.notarize()
        log = json.loads((self.diagnostics / "notarization-log.json").read_text())
        self.assertEqual(log["jobId"], SUBMISSION_ID)
        self.assertEqual(len(self.commands("log")), 3)
        self.assertFalse((self.diagnostics / "notarization-log.partial").exists())

    def test_log_failure_keeps_previous_valid_log_and_does_not_replace_acceptance(self):
        self.notarize()
        log_path = self.diagnostics / "notarization-log.json"
        previous = log_path.read_bytes()
        self.log_events = [(1, '{"incomplete":')] * 3
        self.notarize()
        self.assertEqual(log_path.read_bytes(), previous)
        self.assertEqual(len(self.commands("submit")), 1)
        self.assertEqual(self.state()["status"], "Accepted")
        self.assertFalse((self.diagnostics / "notarization-log.partial").exists())

    def test_processing_deadline_is_bounded_and_keeps_id_for_later_recovery(self):
        self.info_default = "In Progress"
        with self.assertRaises(RuntimeError):
            self.notarize()
        self.assertLessEqual(self.now, 30 * 60)
        self.assertEqual(len(self.commands("submit")), 1)
        self.assertEqual(self.state()["id"], SUBMISSION_ID)
        self.assertFalse(self.commands("log"))
        self.assertFalse(self.stapler_calls())

    def test_successful_pending_reply_resets_consecutive_connection_failure_limit(self):
        self.info_events = [(1, "connection lost")] * 4 + ["In Progress"] + [(1, "connection lost")] * 4 + ["Accepted"]
        self.notarize()
        self.assertEqual(len(self.commands("info")), 10)
        self.assertEqual(len(self.commands("submit")), 1)
        self.assertEqual(self.state()["status"], "Accepted")

    def test_cancellation_during_status_check_preserves_id_and_propagates(self):
        self.info_events = [sign_macos.SigningInterrupted("Signing was interrupted")]
        with self.assertRaises(sign_macos.SigningInterrupted):
            self.notarize()
        self.assertEqual(self.state()["id"], SUBMISSION_ID)
        self.assertEqual(len(self.commands("info")), 1)
        self.assertFalse(self.commands("log"))
        self.assertFalse(self.stapler_calls())


if __name__ == "__main__":
    unittest.main()
