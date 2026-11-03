"""CI state restore contracts; GitHub and artifact responses are local fixtures."""

import contextlib
import hashlib
import importlib.util
import io
import json
import os
from pathlib import Path
import stat
import subprocess
import tempfile
import unittest
import warnings
import zipfile
from unittest.mock import patch


SCRIPT = Path(__file__).resolve().parents[1] / "notary_ci_state.py"
SPEC = importlib.util.spec_from_file_location("notary_ci_state", SCRIPT)
state = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(state)
ID = "11111111-2222-3333-4444-555555555555"
OTHER_ID = "11111111-2222-3333-4444-666666666666"
SHA = "a" * 64
OTHER_SHA = "b" * 64


class StateTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory(prefix="die-ci-state-test-")
        self.addCleanup(temporary.cleanup)
        self.root = Path(temporary.name)
        self.output = io.StringIO()
        self.stack = contextlib.ExitStack()
        self.addCleanup(self.stack.close)
        self.stack.enter_context(patch.dict(os.environ, {
            "RUNNER_TEMP": str(self.root), "GITHUB_REPOSITORY": "owner/repo",
            "GITHUB_RUN_ID": "12345", "GITHUB_RUN_ATTEMPT": "4",
            "GITHUB_SHA": "c" * 40, "CMAKE_OSX_ARCHITECTURES": "arm64", "QT_MAJOR": "6",
            "GH_TOKEN": "secret-api-token",
        }, clear=True))
        self.stack.enter_context(contextlib.redirect_stdout(self.output))
        self.stack.enter_context(contextlib.redirect_stderr(self.output))
        self.value = state.context()

    def record(self, digest=SHA, identifier=ID):
        result = {"sha256": digest, "upload_state": "uncertain" if identifier is None else "submitted"}
        if identifier is not None:
            result["id"] = identifier
            result["status"] = "Accepted"
        return result

    def archive(self, records=None, *, attempt=1, manifest=None, extra=()):
        if records is None:
            records = {("app", SHA): self.record()}
        output = io.BytesIO()
        with zipfile.ZipFile(output, "w", compression=zipfile.ZIP_DEFLATED) as archive:
            archive.writestr("manifest.json", json.dumps(manifest or dict(self.value, run_attempt=attempt)))
            for (kind, digest), record in records.items():
                archive.writestr(f"state/{kind}/{digest}.json", json.dumps(record))
            with warnings.catch_warnings():
                warnings.simplefilter("ignore", UserWarning)
                for name, data in extra:
                    archive.writestr(name, data)
        return output.getvalue()

    def artifact(self, attempt=1, identifier=101, **overrides):
        return {
            "name": state.state_name(self.value, attempt), "id": identifier,
            "size_in_bytes": 1000, "expired": False,
            "workflow_run": {"id": 12345, "head_sha": "c" * 40}, **overrides,
        }

    def api(self, entries, archives=None):
        archives = archives or {101: self.archive()}
        def fake(endpoint, limit):
            if "/runs/" in endpoint:
                return json.dumps({"artifacts": entries}).encode()
            identifier = int(endpoint.split("/")[-2])
            return archives[identifier]
        return fake

    def test_state_normalization_keeps_only_public_recovery_fields(self):
        record = self.record(identifier=ID.upper())
        record.update(upload_started_at="2026-11-03T10:00:00+00:00", upload_returncode=0,
                      upload_timed_out=False, status_checked_at="2026-11-03T10:01:00Z",
                      APPLE_NOTARY_PASSWORD="private-password", command="secret-api-token")
        normalized = state.normalize_state(record, SHA)
        self.assertEqual(normalized["id"], ID)
        self.assertNotIn("private-password", json.dumps(normalized))
        self.assertNotIn("secret-api-token", json.dumps(normalized))
        self.assertEqual(state.normalize_state(self.record(identifier=None))["upload_state"], "uncertain")

    def test_invalid_state_cannot_authorize_a_reuse(self):
        bad_values = [
            {"sha256": "bad"}, {"sha256": OTHER_SHA}, {"id": "bad"}, {"id": None},
            {"upload_state": "unknown"}, {"status": "unknown"},
            {"upload_started_at": "secret-password"}, {"status_checked_at": "2026-11-03"},
            {"upload_returncode": True}, {"upload_timed_out": 1},
        ]
        for change in bad_values:
            with self.subTest(change=change), self.assertRaises(RuntimeError):
                state.normalize_state({**self.record(), **change}, SHA)
        invalid = self.record(identifier=None)
        invalid["status"] = "Accepted"
        with self.assertRaises(RuntimeError):
            state.normalize_state(invalid)

    def test_merge_preserves_known_id_over_uncertainty_and_rejects_conflicts(self):
        records = {}
        state.merge(records, "app", self.record(identifier=None))
        state.merge(records, "app", self.record())
        state.merge(records, "app", self.record(identifier=None))
        self.assertEqual(records[("app", SHA)]["id"], ID)
        state.merge(records, "installer", self.record(identifier=OTHER_ID))
        with self.assertRaisesRegex(RuntimeError, "Conflicting"):
            state.merge(records, "app", self.record(identifier=OTHER_ID))

    def test_first_attempt_creates_empty_restore_without_api(self):
        with patch.dict(os.environ, {"GITHUB_RUN_ATTEMPT": "1"}), patch.object(state, "api_bytes") as api:
            state.restore()
        api.assert_not_called()
        self.assertTrue((self.root / "die-apple-notary-restored").is_dir())

    def test_restore_merges_earlier_attempts_and_isolates_variants(self):
        entries = [self.artifact(3, 103), self.artifact(1), self.artifact(2, 102,
                   name="die-macos-notary-state-x86_64-qt6-attempt-2"), self.artifact(4, 104)]
        archives = {
            101: self.archive({("app", SHA): self.record()}, attempt=1),
            103: self.archive({("app", SHA): self.record(identifier=None),
                              ("installer", OTHER_SHA): self.record(OTHER_SHA, None)}, attempt=3),
        }
        with patch.object(state, "api_bytes", side_effect=self.api(entries, archives)) as api:
            state.restore()
        root = self.root / "die-apple-notary-restored"
        self.assertEqual(json.loads((root / "app" / f"{SHA}.json").read_text())["id"], ID)
        self.assertNotIn("id", json.loads((root / "installer" / f"{OTHER_SHA}.json").read_text()))
        downloads = [call.args[0] for call in api.call_args_list if call.args[0].endswith("/zip")]
        self.assertEqual(downloads, ["repos/owner/repo/actions/artifacts/101/zip", "repos/owner/repo/actions/artifacts/103/zip"])

    def test_missing_history_on_rerun_disables_automatic_upload(self):
        with patch.object(state, "api_bytes", side_effect=self.api([])), \
                self.assertRaisesRegex(RuntimeError, "No prior notarization state"):
            state.restore()
        self.assertFalse((self.root / "die-apple-notary-restored").exists())

    def test_restore_keeps_dmg_and_legacy_installer_ids_separate_for_same_hash(self):
        records = {("dmg", SHA): self.record(), ("installer", SHA): self.record(identifier=OTHER_ID)}
        payload = self.archive(records, extra=[("state/dmg/", b""), ("state/installer/", b"")])
        with patch.object(state, "api_bytes", side_effect=self.api([self.artifact()], {101: payload})):
            state.restore()
        root = self.root / "die-apple-notary-restored"
        self.assertEqual(json.loads((root / "dmg" / f"{SHA}.json").read_text())["id"], ID)
        self.assertEqual(json.loads((root / "installer" / f"{SHA}.json").read_text())["id"], OTHER_ID)

    def test_expired_wrong_run_or_wrong_commit_artifact_is_fatal(self):
        for override in (
            {"expired": True}, {"workflow_run": {"id": 999, "head_sha": "c" * 40}},
            {"workflow_run": {"id": 12345, "head_sha": "d" * 40}},
            {"size_in_bytes": state.MAX_ARCHIVE_BYTES + 1}, {"id": "101"},
        ):
            with self.subTest(override=override), patch.object(state, "api_bytes", side_effect=self.api([
                    self.artifact(**override)])), self.assertRaises(RuntimeError):
                state.restore()
            self.assertFalse((self.root / "die-apple-notary-restored").exists())

    def test_manifest_must_match_every_scope_field(self):
        changes = {"schema_version": 2, "repository": "other/repo", "run_id": "999",
                   "run_attempt": 2, "head_sha": "d" * 40, "arch": "x86_64", "qt_major": "5"}
        for key, wrong in changes.items():
            with self.subTest(key=key), self.assertRaises(RuntimeError):
                state.read_archive(self.archive(manifest={**self.value, "run_attempt": 1, key: wrong}), self.value, 1)

    def test_archive_rejects_traversal_duplicates_symlinks_and_unexpected_members(self):
        symlink = zipfile.ZipInfo(f"state/app/{OTHER_SHA}.json")
        symlink.create_system = 3
        symlink.external_attr = (stat.S_IFLNK | 0o777) << 16
        cases = [("../escape.json", "{}"), ("/absolute.json", "{}"), ("credentials.p12", "secret"),
                 ("state\\app\\file.json", "{}"), ("manifest.json", "{}"), (symlink, "../../outside")]
        for member in cases:
            with self.subTest(name=str(member[0])), self.assertRaises(RuntimeError):
                state.read_archive(self.archive(extra=[member]), self.value, 1)
        self.assertFalse((self.root.parent / "escape.json").exists())

    def test_archive_enforces_size_and_record_hash_limits(self):
        cases = [self.archive({("app", SHA): self.record(OTHER_SHA)}),
                 self.archive(extra=[(f"state/installer/{SHA}.json", " " * (state.MAX_JSON_BYTES + 1))]), b"not a ZIP"]
        for data in cases:
            with self.subTest(size=len(data)), self.assertRaises(RuntimeError):
                state.read_archive(data, self.value, 1)
        with patch.object(state, "MAX_ARCHIVE_BYTES", 10), self.assertRaises(RuntimeError):
            state.read_archive(self.archive(), self.value, 1)
        with patch.object(state, "MAX_MEMBERS", 1), self.assertRaises(RuntimeError):
            state.read_archive(self.archive(), self.value, 1)

    def test_api_failure_never_becomes_an_empty_restore(self):
        with patch.object(state, "api_bytes", side_effect=RuntimeError("GitHub unavailable")), \
                self.assertRaises(RuntimeError):
            state.restore()
        self.assertFalse((self.root / "die-apple-notary-restored").exists())

    def test_paginated_listing_does_not_miss_older_state(self):
        first = [{"name": f"irrelevant-{number}"} for number in range(100)]
        responses = [json.dumps({"artifacts": first}).encode(),
                     json.dumps({"artifacts": [self.artifact()]}).encode()]
        with patch.object(state, "api_bytes", side_effect=responses) as api:
            found = state.prior_artifacts(self.value)
        self.assertEqual(found[0][0], 1)
        self.assertIn("page=2", api.call_args_list[1].args[0])

    def test_incomplete_or_ambiguous_listing_fails(self):
        cases = [{"artifacts": {}}, {"other": []}, {"artifacts": [self.artifact(), self.artifact(identifier=102)]}]
        for payload in cases:
            with self.subTest(payload=payload), patch.object(state, "api_bytes", return_value=json.dumps(payload).encode()), \
                    self.assertRaises(RuntimeError):
                state.prior_artifacts(self.value)
        with patch.object(state, "MAX_PAGES", 1), patch.object(state, "api_bytes", return_value=json.dumps({
                "artifacts": [{"name": "irrelevant"}] * 100}).encode()), self.assertRaises(RuntimeError):
            state.prior_artifacts(self.value)

    def test_conflicting_ids_between_attempts_are_not_silently_selected(self):
        entries = [self.artifact(1), self.artifact(2, 102)]
        archives = {101: self.archive(), 102: self.archive({("app", SHA): self.record(identifier=OTHER_ID)}, attempt=2)}
        with patch.object(state, "api_bytes", side_effect=self.api(entries, archives)), self.assertRaisesRegex(RuntimeError, "Conflicting"):
            state.restore()
        self.assertFalse((self.root / "die-apple-notary-restored").exists())

    def test_save_carries_history_and_current_state_without_sensitive_fields(self):
        restored = self.root / "die-apple-notary-restored/app"
        restored.mkdir(parents=True)
        (restored / f"{SHA}.json").write_text(json.dumps(self.record()))
        current = self.root / "die-apple-notarization/installer"
        current.mkdir(parents=True)
        record = self.record(OTHER_SHA, None)
        record["password"] = "secret-api-token"
        (current / "submission.json").write_text(json.dumps(record))
        (current / "AuthKey.p8").write_text("private key must never be copied")
        state.save()
        saved = self.root / "die-apple-notary-state"
        self.assertEqual(json.loads((saved / "manifest.json").read_text()), self.value)
        self.assertTrue((saved / "state/app" / f"{SHA}.json").exists())
        self.assertTrue((saved / "state/installer" / f"{OTHER_SHA}.json").exists())
        all_text = "".join(path.read_text() for path in saved.rglob("*") if path.is_file())
        self.assertNotIn("secret-api-token", all_text)
        self.assertNotIn("private key", all_text)

    def test_save_empty_history_publishes_an_explicit_manifest(self):
        state.save()
        saved = self.root / "die-apple-notary-state"
        self.assertEqual([path.name for path in saved.iterdir()], ["manifest.json"])

    def test_save_keeps_current_dmg_separate_from_legacy_installer_history(self):
        restored = self.root / "die-apple-notary-restored/installer"
        restored.mkdir(parents=True)
        (restored / f"{SHA}.json").write_text(json.dumps(self.record(identifier=OTHER_ID)))
        current = self.root / "die-apple-notarization/dmg"
        current.mkdir(parents=True)
        (current / "submission.json").write_text(json.dumps(self.record()))
        state.save()
        saved = self.root / "die-apple-notary-state/state"
        self.assertEqual(json.loads((saved / "dmg" / f"{SHA}.json").read_text())["id"], ID)
        self.assertEqual(json.loads((saved / "installer" / f"{SHA}.json").read_text())["id"], OTHER_ID)

    def test_fresh_runner_restores_saved_artifact_and_reuses_id_without_upload(self):
        self.assert_fresh_runner_reuses_id("app")

    def test_fresh_runner_restores_dmg_artifact_and_reuses_id_without_upload(self):
        self.assert_fresh_runner_reuses_id("dmg")

    def assert_fresh_runner_reuses_id(self, kind):
        # Exercise the real save -> ZIP -> restore -> notarize path. Only the
        # remote GitHub and Apple services are replaced, not state integration.
        payload = b"the exact signed payload already submitted to Apple"
        digest = hashlib.sha256(payload).hexdigest()
        current = self.root / "die-apple-notarization" / kind
        current.mkdir(parents=True)
        (current / "submission.json").write_text(json.dumps(self.record(digest)))
        with patch.dict(os.environ, {"GITHUB_RUN_ATTEMPT": "1"}):
            state.save()
        archive_bytes = io.BytesIO()
        saved = self.root / "die-apple-notary-state"
        with zipfile.ZipFile(archive_bytes, "w", compression=zipfile.ZIP_DEFLATED) as archive:
            for path in saved.rglob("*"):
                if path.is_file():
                    archive.write(path, path.relative_to(saved).as_posix())
        fresh = self.root / "fresh-runner"
        fresh.mkdir()
        spec = importlib.util.spec_from_file_location("sign_macos_ci_integration", SCRIPT.with_name("sign_macos.py"))
        signer = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(signer)
        calls = []
        def apple(*args, **kwargs):
            calls.append(args)
            if args[:3] == ("xcrun", "notarytool", "submit"):
                self.fail("A restored matching payload must never be uploaded again")
            if args[:3] == ("xcrun", "notarytool", "info"):
                self.assertEqual(args[3], ID)
                promoted = json.loads((fresh / "die-apple-notarization" / kind / "submission.json").read_text())
                self.assertEqual(promoted["sha256"], digest)
                self.assertEqual(promoted["id"], ID)
                return subprocess.CompletedProcess(args, 0, json.dumps({"id": ID, "status": "Accepted"}), "")
            if args[:3] == ("xcrun", "notarytool", "log"):
                Path(args[-1]).write_text(json.dumps({"jobId": ID, "status": "Accepted", "issues": None}))
            return subprocess.CompletedProcess(args, 0, "", "")
        with patch.dict(os.environ, {
                "RUNNER_TEMP": str(fresh), "GITHUB_RUN_ATTEMPT": "2",
                "APPLE_NOTARY_APPLE_ID": "test@example.invalid", "APPLE_NOTARY_PASSWORD": "fake-password",
                "APPLE_NOTARY_TEAM_ID": "FAKETEAM"}), \
                patch.object(state, "api_bytes", side_effect=self.api([self.artifact()], {101: archive_bytes.getvalue()})), \
                patch.object(signer, "run", side_effect=apple):
            state.restore()
            submission = fresh / ("signed.dmg" if kind == "dmg" else "signed.zip")
            submission.write_bytes(payload)
            target = submission if kind == "dmg" else fresh / "die.app"
            signer.notarize_submission(submission, target, fresh, kind)
        self.assertEqual([call[:3] for call in calls], [
            ("xcrun", "notarytool", "info"), ("xcrun", "notarytool", "log"),
            ("xcrun", "stapler", "staple"), ("xcrun", "stapler", "validate"),
        ])

    def test_invalid_local_state_fails_before_publishing(self):
        active = self.root / "die-apple-notarization/app"
        active.mkdir(parents=True)
        (active / "submission.json").write_text(json.dumps({"id": ID}))
        with self.assertRaises(RuntimeError):
            state.save()
        self.assertFalse((self.root / "die-apple-notary-state").exists())

    def test_dangling_state_symlinks_are_not_treated_as_empty_history(self):
        active = self.root / "die-apple-notarization/app"
        active.mkdir(parents=True)
        (active / "submission.json").symlink_to(self.root / "absent.json")
        with self.assertRaisesRegex(RuntimeError, "symlinked"):
            state.save()
        self.assertFalse((self.root / "die-apple-notary-state").exists())
        (active / "submission.json").unlink()
        (self.root / "die-apple-notary-restored").symlink_to(self.root / "absent-directory")
        with self.assertRaisesRegex(RuntimeError, "symlinked"):
            state.save()

    def test_saved_state_directory_is_not_overwritten(self):
        state.save()
        manifest = self.root / "die-apple-notary-state/manifest.json"
        before = manifest.read_bytes()
        with self.assertRaises(RuntimeError):
            state.save()
        self.assertEqual(manifest.read_bytes(), before)

    def test_api_calls_are_bounded_and_do_not_expose_tokens(self):
        calls = []
        def fake(command, **kwargs):
            calls.append(command)
            self.assertNotIn("secret-api-token", command)
            self.assertLessEqual(kwargs["timeout"], state.API_TIMEOUT)
            kwargs["stdout"].write(b'{"artifacts": []}')
            return subprocess.CompletedProcess(command, 0, b"", b"secret-api-token")
        with patch.object(state.subprocess, "run", side_effect=fake):
            self.assertEqual(state.api_bytes("repos/owner/repo/actions/runs/12345/artifacts", 100), b'{"artifacts": []}')
        self.assertEqual(calls[0][:2], ["gh", "api"])
        self.assertNotIn("secret-api-token", self.output.getvalue())
        with patch.object(state.subprocess, "run", return_value=subprocess.CompletedProcess([], 1, b"", b"secret-api-token")), \
                self.assertRaises(RuntimeError) as error:
            state.api_bytes("endpoint", 100)
        self.assertNotIn("secret-api-token", str(error.exception))
        with patch.object(state.subprocess, "run", side_effect=subprocess.TimeoutExpired(["secret-api-token"], 45)), \
                self.assertRaises(RuntimeError) as error:
            state.api_bytes("endpoint", 100)
        self.assertNotIn("secret-api-token", str(error.exception))

    def test_api_size_and_total_deadline_are_enforced(self):
        def fake(command, **kwargs):
            kwargs["stdout"].write(b"too long")
            return subprocess.CompletedProcess(command, 0, b"", b"")
        with patch.object(state.subprocess, "run", side_effect=fake), self.assertRaises(RuntimeError):
            state.api_bytes("endpoint", 1)
        with patch.object(state, "_deadline", 0), patch.object(state.subprocess, "run") as command, self.assertRaises(RuntimeError):
            state.api_bytes("endpoint", 100)
        command.assert_not_called()


if __name__ == "__main__":
    unittest.main()
