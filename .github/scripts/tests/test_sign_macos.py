"""Apple signing checks with fake credentials and mocked Apple tools only."""

import contextlib
import importlib.util
import io
import json
import os
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest
from unittest.mock import Mock, patch


SCRIPT = Path(__file__).resolve().parents[1] / "sign_macos.py"
SPEC = importlib.util.spec_from_file_location("sign_macos", SCRIPT)
sign_macos = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(sign_macos)

CERTIFICATE = {
    "APPLE_CERTIFICATE_P12_BASE64": "ZmFrZS1jZXJ0aWZpY2F0ZQ==",
    "APPLE_CERTIFICATE_PASSWORD": "fake-certificate-password",
}
API_KEY = {
    "APPLE_NOTARY_KEY_P8": "fake-api-private-key",
    "APPLE_NOTARY_KEY_ID": "FAKEKEYID",
    "APPLE_NOTARY_ISSUER_ID": "fake-api-issuer",
}
APPLE_ID = {
    "APPLE_NOTARY_APPLE_ID": "fake-apple-id@example.invalid",
    "APPLE_NOTARY_PASSWORD": "fake-app-specific-password",
    "APPLE_NOTARY_TEAM_ID": "FAKETEAMID",
}
SUBMISSION_ID = "11111111-2222-3333-4444-555555555555"
DMG_SUBMISSION_ID = "11111111-2222-3333-4444-666666666666"
APP_FINGERPRINT = "A" * 40
OTHER_FINGERPRINT = "B" * 40
APP_NAME = "Developer ID Application: Test Developer (FAKETEAMID)"


class SigningTestCase(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory(prefix="die-signing-test-")
        self.addCleanup(temporary.cleanup)
        self.root = Path(temporary.name)
        self.output = self.root / "output"
        self.summary = self.root / "summary"
        environment = patch.dict(os.environ, {
            "RUNNER_TEMP": str(self.root),
            "GITHUB_OUTPUT": str(self.output),
            "GITHUB_STEP_SUMMARY": str(self.summary),
        }, clear=True)
        environment.start()
        self.addCleanup(environment.stop)

    def cli(self, *arguments):
        stdout, stderr = io.StringIO(), io.StringIO()
        with patch("sys.argv", [str(SCRIPT), *arguments]), \
                patch.object(sign_macos.signal, "signal"), \
                patch.object(sign_macos.os, "umask"), \
                contextlib.redirect_stdout(stdout), contextlib.redirect_stderr(stderr):
            result = sign_macos.main()
        return result, stdout.getvalue(), stderr.getvalue()


class ConfigurationTests(SigningTestCase):
    def test_complete_api_and_apple_id_modes(self):
        for credentials, mode in ((API_KEY, "api-key"), (APPLE_ID, "apple-id")):
            with self.subTest(mode=mode), patch.dict(os.environ, {**CERTIFICATE, **credentials}):
                self.assertTrue(sign_macos.check_config())
                self.assertTrue(sign_macos.check_config(allow_unsigned=True))
                self.assertEqual(sign_macos.notary_mode(), mode)

    def test_complete_api_key_has_priority(self):
        with patch.dict(os.environ, {**CERTIFICATE, **API_KEY, **APPLE_ID}):
            self.assertTrue(sign_macos.check_config())
            self.assertEqual(sign_macos.notary_mode(), "api-key")

    def test_complete_apple_id_can_replace_incomplete_api_key(self):
        with patch.dict(os.environ, {**CERTIFICATE, **APPLE_ID, "APPLE_NOTARY_KEY_ID": "unused"}):
            self.assertTrue(sign_macos.check_config())
            self.assertEqual(sign_macos.notary_mode(), "apple-id")

    def test_no_credentials_allowed_only_when_requested(self):
        self.assertFalse(sign_macos.check_config(allow_unsigned=True))
        with self.assertRaisesRegex(RuntimeError, "Missing Apple signing configuration"):
            sign_macos.check_config()

    def test_identity_and_whitespace_do_not_enable_signing(self):
        fields = dict.fromkeys(sign_macos.CORE_CREDENTIAL_FIELDS, " \n\t")
        with patch.dict(os.environ, {**fields, "APPLE_SIGNING_IDENTITY": "fake optional identity"}):
            self.assertFalse(sign_macos.check_config(allow_unsigned=True))
            with self.assertRaises(RuntimeError):
                sign_macos.check_config()

    def test_any_single_credential_is_incomplete_even_in_optional_mode(self):
        for field in sign_macos.CORE_CREDENTIAL_FIELDS:
            with self.subTest(field=field), patch.dict(os.environ, {field: "fake-value"}):
                with self.assertRaises(RuntimeError):
                    sign_macos.check_config(allow_unsigned=True)

    def test_each_required_field_is_checked_in_both_modes(self):
        for credentials in (API_KEY, APPLE_ID):
            complete = {**CERTIFICATE, **credentials}
            for missing in complete:
                partial = {name: value for name, value in complete.items() if name != missing}
                with self.subTest(missing=missing), patch.dict(os.environ, partial):
                    with self.assertRaises(RuntimeError):
                        sign_macos.check_config(allow_unsigned=True)

    def test_optional_cli_reports_unsigned_artifacts_and_output(self):
        result, stdout, stderr = self.cli("check", "--allow-unsigned")
        self.assertEqual(result, 0)
        self.assertIn("::notice::", stdout)
        self.assertIn("unsigned artifacts", stdout)
        self.assertEqual(stderr, "")
        self.assertEqual(self.output.read_text(), "enabled=false\n")
        self.assertIn("unsigned artifacts", self.summary.read_text())

    def test_complete_cli_reports_enabled_without_credentials(self):
        with patch.dict(os.environ, {**CERTIFICATE, **APPLE_ID}):
            result, stdout, stderr = self.cli("check", "--allow-unsigned")
        self.assertEqual(result, 0)
        self.assertEqual(self.output.read_text(), "enabled=true\n")
        self.assertEqual(stdout + stderr, "")

    def test_partial_cli_fails_without_enabled_output_or_secret_values(self):
        with patch.dict(os.environ, {"APPLE_CERTIFICATE_PASSWORD": "do-not-print-this-secret"}):
            result, stdout, stderr = self.cli("check", "--allow-unsigned")
        self.assertEqual(result, 1)
        self.assertIn("::error::", stderr)
        self.assertNotIn("do-not-print-this-secret", stdout + stderr)
        self.assertFalse(self.output.exists())

    def test_strict_cli_rejects_empty_configuration(self):
        result, _, stderr = self.cli("check")
        self.assertEqual(result, 1)
        self.assertIn("::error::", stderr)
        self.assertFalse(self.output.exists())

    def test_allow_unsigned_is_not_a_signing_bypass(self):
        with patch("sys.argv", [str(SCRIPT), "sign", "archive.zip", "--allow-unsigned"]), \
                patch.object(sign_macos.os, "umask"), contextlib.redirect_stderr(io.StringIO()), \
                self.assertRaises(SystemExit) as error:
            sign_macos.main()
        self.assertEqual(error.exception.code, 2)
        with self.assertRaises(RuntimeError):
            sign_macos.sign_package(self.root / "absent.zip", self.root / "absent.dmg")

    def test_only_check_writes_enabled_output(self):
        with patch.object(sign_macos, "cleanup") as cleanup:
            self.assertEqual(self.cli("cleanup")[0], 0)
            cleanup.assert_called_once_with()
        with patch.object(sign_macos, "sign_package") as sign_package:
            self.assertEqual(self.cli("sign", "archive.zip", "disk_image.dmg")[0], 0)
            sign_package.assert_called_once_with(Path("archive.zip"), Path("disk_image.dmg"))
        self.assertFalse(self.output.exists())

    def test_sign_cli_requires_both_paths(self):
        with patch.object(sign_macos, "sign_package") as sign_package, \
                contextlib.redirect_stderr(io.StringIO()), self.assertRaises(SystemExit) as error:
            self.cli("sign", "archive.zip")
        self.assertEqual(error.exception.code, 2)
        sign_package.assert_not_called()


class IdentityTests(SigningTestCase):
    def invoke_import(self, identities=None, selector=None):
        root = self.root / "identity-import"
        root.mkdir(exist_ok=True)
        calls = []
        if identities is None:
            identities = f'1) {APP_FINGERPRINT} "{APP_NAME}"\n'

        def fake_run(*args, **kwargs):
            calls.append(args)
            output = ""
            if args[:2] == ("security", "find-identity"):
                output = identities
            elif args == ("security", "list-keychains", "-d", "user"):
                output = '"/fake/original.keychain-db"\n'
            return subprocess.CompletedProcess(args, 0, output, "")

        environment = dict(CERTIFICATE)
        if selector is not None:
            environment["APPLE_SIGNING_IDENTITY"] = selector
        with patch.dict(os.environ, environment), patch.object(sign_macos, "run", side_effect=fake_run):
            result = sign_macos.import_identities(root)
        return result, calls

    def test_import_only_requires_application_certificate_and_codesign_access(self):
        (keychain, application), calls = self.invoke_import()
        self.assertEqual(application, APP_FINGERPRINT)
        self.assertIn(("security", "find-identity", "-v", "-p", "codesigning", keychain), calls)
        imports = [call for call in calls if call[:2] == ("security", "import")]
        self.assertEqual(len(imports), 1)
        call = imports[0]
        self.assertEqual(Path(call[2]).read_bytes(), b"fake-certificate")
        self.assertEqual(call[call.index("-P") + 1], CERTIFICATE["APPLE_CERTIFICATE_PASSWORD"])
        self.assertEqual(call[call.index("-T") + 1], "/usr/bin/codesign")
        self.assertEqual(call[call.index("-k") + 1], keychain)
        partition = next(call for call in calls if call[:2] == ("security", "set-key-partition-list"))
        self.assertEqual(partition[partition.index("-S") + 1], "apple-tool:,apple:")

    def test_application_name_or_fingerprint_selects_identity(self):
        identities = (f'1) {APP_FINGERPRINT} "{APP_NAME}"\n'
                      f'2) {OTHER_FINGERPRINT} "Developer ID Application: Other Developer (OTHERTEAM)"\n')
        for selector in (APP_NAME, APP_FINGERPRINT.lower()):
            with self.subTest(selector=selector):
                result, _ = self.invoke_import(identities, selector)
                self.assertEqual(result[1], APP_FINGERPRINT)

    def test_missing_ambiguous_or_unmatched_application_identity_is_rejected(self):
        identities = f'1) {APP_FINGERPRINT} "{APP_NAME}"\n'
        cases = (
            (f'1) {OTHER_FINGERPRINT} "Developer ID Installer: Test Developer"\n', None),
            (identities + f'2) {OTHER_FINGERPRINT} "Developer ID Application: Other Developer"\n', None),
            (identities, "Developer ID Application: Missing"),
        )
        for output, selector in cases:
            with self.subTest(selector=selector, output=output), \
                    self.assertRaisesRegex(RuntimeError, "Developer ID Application identity"):
                self.invoke_import(output, selector)


class NotarizationTests(SigningTestCase):
    def invoke_notarize(self, credentials, response, *, submit_code=0, log_code=0, disk_image=False):
        signing_root = self.root / "signing"
        signing_root.mkdir(exist_ok=True)
        app = self.root / "die.app"
        app.mkdir(exist_ok=True)
        package = self.root / "die.dmg"
        package.write_bytes(b"fake signed disk image")
        kind = "dmg" if disk_image else "app"
        shutil.rmtree(self.root / "die-apple-notarization" / kind, ignore_errors=True)
        calls = []
        clock = [0.0]

        def fake_run(*args, **kwargs):
            calls.append(args)
            if args[:3] == ("ditto", "-c", "-k"):
                Path(args[-1]).write_bytes(b"fake signed app ZIP")
            if args[:3] == ("xcrun", "notarytool", "submit"):
                return subprocess.CompletedProcess(args, submit_code, response, "fake-secret-stderr")
            if args[:3] == ("xcrun", "notarytool", "info"):
                return subprocess.CompletedProcess(args, 0, response, "fake-secret-stderr")
            if args[:3] == ("xcrun", "notarytool", "log"):
                if log_code == 0:
                    Path(args[-1]).write_text(json.dumps({"jobId": SUBMISSION_ID, "status": "Accepted", "issues": None}))
                return subprocess.CompletedProcess(args, log_code, "", "fake-secret-stderr")
            return subprocess.CompletedProcess(args, 0, "", "")

        stdout, stderr = io.StringIO(), io.StringIO()
        error = None
        with patch.dict(os.environ, {**CERTIFICATE, **credentials}), \
                patch.object(sign_macos, "run", side_effect=fake_run), \
                patch.object(sign_macos.time, "monotonic", side_effect=lambda: clock[0]), \
                patch.object(sign_macos.time, "sleep", side_effect=lambda seconds: clock.__setitem__(0, clock[0] + seconds)), \
                contextlib.redirect_stdout(stdout), contextlib.redirect_stderr(stderr):
            try:
                if disk_image:
                    sign_macos.notarize_disk_image(package, signing_root)
                else:
                    sign_macos.notarize(app, signing_root)
            except RuntimeError as caught:
                error = caught
        return calls, stdout.getvalue() + stderr.getvalue(), error

    def test_both_auth_modes_reuse_credentials_for_submit_info_and_log(self):
        response = json.dumps({"id": SUBMISSION_ID, "status": "Accepted"})
        for credentials, flag in ((API_KEY, "--key"), (APPLE_ID, "--apple-id"),
                                  ({**API_KEY, **APPLE_ID}, "--key")):
            with self.subTest(flag=flag, both=len(credentials) == 6):
                calls, output, error = self.invoke_notarize(credentials, response)
                self.assertIsNone(error)
                submit = next(call for call in calls if call[:3] == ("xcrun", "notarytool", "submit"))
                info = next(call for call in calls if call[:3] == ("xcrun", "notarytool", "info"))
                log = next(call for call in calls if call[:3] == ("xcrun", "notarytool", "log"))
                auth = submit[4:submit.index("--no-wait")]
                self.assertEqual(auth, info[4:info.index("--output-format")])
                self.assertEqual(auth, log[4:-1])
                self.assertEqual(auth[0], flag)
                if flag == "--apple-id":
                    self.assertEqual(auth, ("--apple-id", APPLE_ID["APPLE_NOTARY_APPLE_ID"],
                                          "--password", APPLE_ID["APPLE_NOTARY_PASSWORD"],
                                          "--team-id", APPLE_ID["APPLE_NOTARY_TEAM_ID"]))
                else:
                    self.assertNotIn("--password", auth)
                    self.assertEqual(Path(auth[1]).read_text(), API_KEY["APPLE_NOTARY_KEY_P8"] + "\n")
                self.assertTrue(any(call[:3] == ("xcrun", "stapler", "staple") for call in calls))
                self.assertTrue(any(call[:3] == ("xcrun", "stapler", "validate") for call in calls))
                self.assertTrue(any(call[0] == "spctl" for call in calls))
                for secret in credentials.values():
                    self.assertNotIn(secret, output)

    def test_apple_id_does_not_write_an_api_key(self):
        calls, _, error = self.invoke_notarize(
            APPLE_ID, json.dumps({"id": SUBMISSION_ID, "status": "Accepted"}))
        self.assertIsNone(error)
        self.assertFalse((self.root / "signing/AuthKey.p8").exists())

    def test_rejected_or_incomplete_submissions_are_not_stapled(self):
        responses = (
            (json.dumps({"id": SUBMISSION_ID, "status": "Invalid"}), 0),
            (json.dumps({"id": SUBMISSION_ID, "status": "In Progress"}), 1),
            (json.dumps({"id": "not-a-submission-id", "status": "Accepted"}), 0),
            ("fake-secret-invalid-json", 1),
            ("[]", 0),
        )
        for response, code in responses:
            with self.subTest(response=response, code=code):
                calls, output, error = self.invoke_notarize(APPLE_ID, response, submit_code=code)
                self.assertIsInstance(error, RuntimeError)
                self.assertFalse(any(call[:2] == ("xcrun", "stapler") for call in calls))
                self.assertNotIn("fake-secret", output + str(error))

    def test_missing_notarization_log_warns_without_blocking_accepted_submission(self):
        calls, output, error = self.invoke_notarize(
            API_KEY, json.dumps({"id": SUBMISSION_ID, "status": "Accepted"}), log_code=1)
        self.assertIsNone(error)
        self.assertIn("::warning::", output)
        self.assertNotIn("fake-secret", output)
        self.assertTrue(any(call[:3] == ("xcrun", "stapler", "staple") for call in calls))

    def test_disk_image_submitted_directly_and_checked_as_disk_image(self):
        calls, output, error = self.invoke_notarize(
            APPLE_ID, json.dumps({"id": SUBMISSION_ID, "status": "Accepted"}), disk_image=True)
        self.assertIsNone(error)
        package = self.root / "die.dmg"
        self.assertFalse(any(call[0] == "ditto" for call in calls))
        submit = next(call for call in calls if call[:3] == ("xcrun", "notarytool", "submit"))
        self.assertEqual(submit[3], package)
        self.assertIn(("xcrun", "stapler", "staple", package), calls)
        self.assertIn(("xcrun", "stapler", "validate", package), calls)
        self.assertIn(("codesign", "--verify", "--strict", "--verbose=2", package), calls)
        self.assertIn(("hdiutil", "verify", package), calls)
        self.assertIn(("spctl", "--assess", "--type", "open", "--context", "context:primary-signature", "--verbose=2", package), calls)
        for value in APPLE_ID.values():
            self.assertNotIn(value, output)

    def test_app_and_disk_image_diagnostics_do_not_overwrite_each_other(self):
        response = json.dumps({"id": SUBMISSION_ID, "status": "Accepted"})
        for kind in ("app", "dmg"):
            calls, _, error = self.invoke_notarize(APPLE_ID, response, disk_image=kind == "dmg")
            self.assertIsNone(error)
            diagnostics = self.root / "die-apple-notarization" / kind
            self.assertEqual(json.loads((diagnostics / "submission.json").read_text())["status"], "Accepted")
            log = next(call for call in calls if call[:3] == ("xcrun", "notarytool", "log"))
            self.assertEqual(log[3], SUBMISSION_ID)
            self.assertEqual(json.loads((diagnostics / "notarization-log.json").read_text())["jobId"], SUBMISSION_ID)
        self.assertTrue((self.root / "die-apple-notarization/app/submission.json").is_file())


class PackagePairTests(SigningTestCase):
    def setUp(self):
        super().setUp()
        self.archive = self.root / "package.zip"
        self.disk_image = self.root / "package.dmg"
        self.archive.write_bytes(b"original ZIP")
        self.disk_image.write_bytes(b"original DMG")
        fixture = self.root / "fixture"
        binaries = fixture / "die_mac_portable/die.app/Contents/MacOS"
        binaries.mkdir(parents=True)
        for name in ("die", "diec"):
            (binaries / name).write_bytes(b"fake executable")
        self.snapshots = {self.archive: fixture}
        self.calls = []
        self.output_fault = None
        self.disk_image_status = "Accepted"
        self.signed_app = None

    def fake_import(self, root):
        keychain = root / "signing.keychain-db"
        keychain.write_bytes(b"fake keychain")
        (root / "certificate.p12").write_bytes(b"fake private certificate")
        return keychain, APP_FINGERPRINT

    def fake_sign_app(self, app, root, keychain, identity):
        self.assertEqual(identity, APP_FINGERPRINT)
        self.signed_app = app
        (app / "signed-marker").write_bytes(b"signed application")

    def write_output(self, path, kind, content):
        if self.output_fault == (kind, "missing"):
            return
        path.write_bytes(b"" if self.output_fault == (kind, "empty") else content)

    def fake_run(self, *args, **kwargs):
        self.calls.append(args)
        output = ""
        if args[:3] == ("ditto", "-x", "-k"):
            shutil.copytree(self.snapshots[Path(args[3])], args[4], dirs_exist_ok=True)
        elif args[:3] == ("ditto", "-c", "-k"):
            source, target = Path(args[-2]), Path(args[-1])
            snapshot = self.root / "snapshots" / str(len(self.calls))
            shutil.copytree(source, snapshot / source.name)
            self.snapshots[target] = snapshot
            kind = "zip" if source.name == "die_mac_portable" else "submission"
            self.write_output(target, kind, b"signed ZIP")
        elif args[0] == "ditto":
            source, target = Path(args[1]), Path(args[2])
            self.assertTrue((source / "signed-marker").is_file())
            self.assertTrue((source / "stapled-marker").is_file())
            shutil.copytree(source, target, symlinks=True)
        elif args[:2] == ("hdiutil", "create"):
            self.assertEqual(kwargs["timeout"], 900)
            self.assertEqual(kwargs["diagnose_after"], (60, 300, 840))
            staging = Path(args[args.index("-srcfolder") + 1])
            app = staging / self.signed_app.name
            self.assertTrue((app / "signed-marker").is_file())
            self.assertTrue((app / "stapled-marker").is_file())
            self.assertEqual({p.name for p in staging.iterdir()}, {self.signed_app.name, "Applications"})
            self.assertEqual((staging / "Applications").readlink(), Path("/Applications"))
            self.assertEqual(staging.stat().st_mode & 0o777, 0o755)
            self.assertEqual(args[args.index("-format") + 1], "UDZO")
            self.assertEqual(args[args.index("-fs") + 1], "HFS+")
            self.write_output(Path(args[-1]), "dmg", b"unsigned DMG")
        elif args[0] == "codesign" and "--sign" in args:
            self.assertEqual(args[args.index("--sign") + 1], APP_FINGERPRINT)
            self.assertIn("--keychain", args)
            self.assertIn("--timestamp", args)
            target = Path(args[-1])
            self.assertEqual(target.suffix, ".dmg")
            self.assertEqual(target.read_bytes(), b"unsigned DMG")
            target.write_bytes(b"signed DMG")
        elif args[:3] == ("xcrun", "notarytool", "submit"):
            self.assertEqual(kwargs["timeout"], 35 * 60)
            self.assertNotIn("diagnose_after", kwargs)
            # Neither original may be replaced before the second submission succeeds.
            self.assertEqual(self.archive.read_bytes(), b"original ZIP")
            self.assertEqual(self.disk_image.read_bytes(), b"original DMG")
            self.assertIn("--no-wait", args)
            self.assertNotIn("--wait", args)
            if Path(args[3]).suffix == ".dmg":
                self.assertEqual(Path(args[3]).read_bytes(), b"signed DMG")
            submission_id = DMG_SUBMISSION_ID if Path(args[3]).suffix == ".dmg" else SUBMISSION_ID
            output = json.dumps({"id": submission_id, "status": "In Progress"})
        elif args[:3] == ("xcrun", "notarytool", "info"):
            status = self.disk_image_status if args[3] == DMG_SUBMISSION_ID else "Accepted"
            output = json.dumps({"id": args[3], "status": status})
        elif args[:3] == ("xcrun", "notarytool", "log"):
            status = self.disk_image_status if args[3] == DMG_SUBMISSION_ID else "Accepted"
            Path(args[-1]).write_text(json.dumps({"jobId": args[3], "status": status, "issues": None}))
        elif args[:3] == ("xcrun", "stapler", "staple"):
            target = Path(args[-1])
            if target.is_dir():
                (target / "stapled-marker").write_bytes(b"stapled application")
        elif args[:3] == ("xcrun", "stapler", "validate") and Path(args[-1]).is_dir():
            self.assertTrue((Path(args[-1]) / "stapled-marker").is_file())
        elif Path(args[0]).name == "diec":
            self.assertFalse(any(name.startswith(("APPLE_", "DYLD_", "QT_", "QML"))
                                 for name in kwargs["env"]))
        return subprocess.CompletedProcess(args, 0, output, "fake-tool-secret")

    def invoke_signing(self):
        stdout, stderr = io.StringIO(), io.StringIO()
        with patch.dict(os.environ, {**CERTIFICATE, **APPLE_ID}), \
                patch.object(sign_macos, "import_identities", side_effect=self.fake_import), \
                patch.object(sign_macos, "sign_app", side_effect=self.fake_sign_app), \
                patch.object(sign_macos, "run", side_effect=self.fake_run), \
                contextlib.redirect_stdout(stdout), contextlib.redirect_stderr(stderr):
            sign_macos.sign_package(self.archive, self.disk_image)
        return stdout.getvalue() + stderr.getvalue()

    def assert_cleaned(self):
        self.assertFalse(sign_macos.signing_dir().exists())
        self.assertFalse(self.archive.with_suffix(".signed.zip").exists())
        self.assertFalse(self.disk_image.with_suffix(".signed.dmg").exists())
        self.assertTrue(any(call[:2] == ("security", "delete-keychain") for call in self.calls))

    def test_both_outputs_replaced_after_both_notarizations_are_accepted(self):
        output = self.invoke_signing()
        self.assertEqual(self.archive.read_bytes(), b"signed ZIP")
        self.assertEqual(self.disk_image.read_bytes(), b"signed DMG")
        submissions = [call for call in self.calls if call[:3] == ("xcrun", "notarytool", "submit")]
        self.assertEqual([Path(call[3]).suffix for call in submissions], [".zip", ".dmg"])
        self.assertTrue(any(call[:4] == ("spctl", "--assess", "--type", "open") for call in self.calls))
        for secret in (*CERTIFICATE.values(), *APPLE_ID.values(), "fake-tool-secret"):
            self.assertNotIn(secret, output)
        self.assert_cleaned()

    def test_rejected_disk_image_preserves_both_originals_and_cleans_credentials(self):
        self.disk_image_status = "Invalid"
        with self.assertRaisesRegex(RuntimeError, "Apple did not accept"):
            self.invoke_signing()
        self.assertEqual(self.archive.read_bytes(), b"original ZIP")
        self.assertEqual(self.disk_image.read_bytes(), b"original DMG")
        self.assertFalse(any(call[:3] == ("xcrun", "stapler", "staple") and
                             Path(call[-1]).suffix == ".dmg" for call in self.calls))
        self.assert_cleaned()

    def test_disk_image_failures_preserve_both_originals_and_cleanup(self):
        phases = (
            "dmg: stage signed and stapled app",
            "dmg: create compressed disk image",
            "dmg: verify disk image before signing",
            "dmg: sign disk image",
            "dmg: verify signature before submission",
            "dmg: stapler staple, attempt 1/3",
            "dmg: stapler validate, attempt 1/3",
            "dmg: verify signature after stapling",
            "dmg: verify final disk image",
            "dmg: Gatekeeper assessment",
        )
        fake_run = self.fake_run
        for phase in phases:
            with self.subTest(phase=phase):
                self.calls.clear()
                shutil.rmtree(self.root / "die-apple-notarization", ignore_errors=True)
                shutil.rmtree(self.root / "snapshots", ignore_errors=True)

                def fail_phase(*args, **kwargs):
                    if kwargs.get("phase") == phase:
                        raise RuntimeError("simulated DMG tool failure")
                    return fake_run(*args, **kwargs)

                with patch.object(self, "fake_run", side_effect=fail_phase), \
                        self.assertRaisesRegex(RuntimeError, "simulated DMG tool failure"):
                    self.invoke_signing()
                self.assertEqual(self.archive.read_bytes(), b"original ZIP")
                self.assertEqual(self.disk_image.read_bytes(), b"original DMG")
                self.assert_cleaned()

    def test_disk_image_creation_timeout_cleans_up_without_submitting_dmg(self):
        fake_run = self.fake_run

        def timeout_create(*args, **kwargs):
            if args[:2] == ("hdiutil", "create"):
                raise sign_macos.CommandTimeout("simulated disk image timeout")
            return fake_run(*args, **kwargs)

        with patch.object(self, "fake_run", side_effect=timeout_create), \
                self.assertRaises(sign_macos.CommandTimeout):
            self.invoke_signing()
        self.assertEqual(self.archive.read_bytes(), b"original ZIP")
        self.assertEqual(self.disk_image.read_bytes(), b"original DMG")
        self.assertFalse(any(call[:3] == ("xcrun", "notarytool", "submit") and
                             Path(call[3]).suffix == ".dmg" for call in self.calls))
        self.assert_cleaned()

    def test_stapler_timeouts_preserve_outputs_and_cleanup_runs_outside_deadline(self):
        fake_run = self.fake_run

        def timeout_after_acceptance(*args, **kwargs):
            if args[:3] == ("xcrun", "stapler", "validate"):
                raise sign_macos.CommandTimeout("simulated stapler timeout")
            if args[:2] == ("security", "delete-keychain"):
                self.assertIsNone(sign_macos._signing_deadline)
                self.assertLessEqual(kwargs["timeout"], 60)
            return fake_run(*args, **kwargs)

        with patch.object(self, "fake_run", side_effect=timeout_after_acceptance), \
                patch.object(sign_macos.time, "sleep"), self.assertRaises(RuntimeError):
            self.invoke_signing()
        self.assertEqual(self.archive.read_bytes(), b"original ZIP")
        self.assertEqual(self.disk_image.read_bytes(), b"original DMG")
        self.assert_cleaned()

    def test_missing_or_empty_generated_output_preserves_both_originals(self):
        for kind in ("zip", "dmg"):
            for failure in ("missing", "empty"):
                with self.subTest(kind=kind, failure=failure):
                    self.output_fault = (kind, failure)
                    with self.assertRaisesRegex(RuntimeError, "nonempty output"):
                        self.invoke_signing()
                    self.assertEqual(self.archive.read_bytes(), b"original ZIP")
                    self.assertEqual(self.disk_image.read_bytes(), b"original DMG")
                    self.assert_cleaned()

    def test_missing_or_empty_input_is_rejected_before_import(self):
        for target in (self.archive, self.disk_image):
            original = target.read_bytes()
            for failure in ("missing", "empty"):
                with self.subTest(target=target.name, failure=failure):
                    target.unlink(missing_ok=True)
                    if failure == "empty":
                        target.write_bytes(b"")
                    with patch.dict(os.environ, {**CERTIFICATE, **APPLE_ID}), \
                            patch.object(sign_macos, "import_identities") as identity_import, \
                            self.assertRaises((RuntimeError, OSError)):
                        sign_macos.sign_package(self.archive, self.disk_image)
                    identity_import.assert_not_called()
                    self.assertFalse(sign_macos.signing_dir().exists())
                    target.write_bytes(original)


class CredentialHandlingTests(SigningTestCase):
    def test_sensitive_tool_failures_do_not_echo_commands_or_stderr(self):
        for command in (("security", "import", "--password", "fake-command-secret"),
                        ("xcrun", "notarytool", "submit", "--password", "fake-command-secret")):
            output = io.StringIO()
            process = Mock(returncode=1)
            process.communicate.return_value = ("fake-stdout-secret", "fake-stderr-secret")
            with self.subTest(tool=command[0]), patch.object(sign_macos.subprocess, "Popen", return_value=process), \
                    contextlib.redirect_stdout(output), contextlib.redirect_stderr(output), \
                    self.assertRaises(RuntimeError) as error:
                sign_macos.run(*command)
            self.assertNotIn("secret", output.getvalue() + str(error.exception))

    def test_failed_identity_import_cleans_credentials_and_keeps_original_archive(self):
        archive = self.root / "package.zip"
        archive.write_bytes(b"original unsigned archive")
        disk_image = self.root / "package.dmg"
        disk_image.write_bytes(b"original unsigned disk image")

        def failed_import(root):
            (root / "certificate.p12").write_bytes(b"fake-private-certificate")
            raise RuntimeError("simulated identity import failure")

        with patch.dict(os.environ, {**CERTIFICATE, **APPLE_ID}), \
                patch.object(sign_macos, "import_identities", side_effect=failed_import), \
                patch.object(sign_macos, "run", side_effect=AssertionError("Unexpected external tool")), \
                self.assertRaisesRegex(RuntimeError, "identity import failure"):
            sign_macos.sign_package(archive, disk_image)
        self.assertFalse(sign_macos.signing_dir().exists())
        self.assertEqual(archive.read_bytes(), b"original unsigned archive")
        self.assertEqual(disk_image.read_bytes(), b"original unsigned disk image")
        self.assertFalse(archive.with_suffix(".signed.zip").exists())
        self.assertFalse(disk_image.with_suffix(".signed.dmg").exists())


if __name__ == "__main__":
    unittest.main()
