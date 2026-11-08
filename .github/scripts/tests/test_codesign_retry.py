"""Apple timestamp retries use fake codesign results, never real signing keys."""

import contextlib
import importlib.util
import io
import os
from pathlib import Path
import plistlib
import subprocess
import tempfile
import unittest
from unittest.mock import patch


SCRIPT = Path(__file__).resolve().parents[1] / "sign_macos.py"
SPEC = importlib.util.spec_from_file_location("sign_macos_codesign_retry", SCRIPT)
sign_macos = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(sign_macos)
UNAVAILABLE = "The timestamp service is not available."


class CodesignRetryTests(unittest.TestCase):
    def setUp(self):
        self.stack = contextlib.ExitStack()
        self.addCleanup(self.stack.close)
        self.stack.enter_context(contextlib.redirect_stdout(io.StringIO()))
        self.stack.enter_context(contextlib.redirect_stderr(io.StringIO()))
        self.stack.enter_context(patch.object(sign_macos, "_signing_deadline", None))

    def result(self, code, error=""):
        return subprocess.CompletedProcess([], code, "", error)

    def args(self, target="die.app"):
        return ("--force", "--sign", "fake-identity", "--keychain", "fake-keychain",
                "--timestamp", target)

    def test_service_failure_recovers_for_bundle_and_disk_image_without_changing_arguments(self):
        for target in ("die.app", "die.dmg"):
            with self.subTest(target=target), \
                    patch.object(sign_macos, "run", side_effect=[self.result(1, UNAVAILABLE), self.result(0)]) as run, \
                    patch.object(sign_macos.time, "sleep") as sleep:
                args = self.args(target)
                sign_macos.sign_with_timestamp(*args, phase="Sign distribution")
                self.assertEqual(run.call_count, 2)
                self.assertTrue(all(call.args == ("codesign", *args) for call in run.call_args_list))
                self.assertTrue(all(call.kwargs["check"] is False for call in run.call_args_list))
                self.assertTrue(all(call.kwargs["timeout"] == 120 for call in run.call_args_list))
                sleep.assert_called_once_with(15)

    def test_exhausted_timestamp_service_failure_stops_after_three_attempts(self):
        with patch.object(sign_macos, "run", return_value=self.result(1, UNAVAILABLE)) as run, \
                patch.object(sign_macos.time, "sleep") as sleep, \
                self.assertRaisesRegex(RuntimeError, "remained unavailable"):
            sign_macos.sign_with_timestamp(*self.args(), phase="Sign distribution")
        self.assertEqual(run.call_count, 3)
        self.assertEqual([call.args[0] for call in sleep.call_args_list], [15, 30])

    def test_other_timestamp_and_keychain_errors_are_not_retried(self):
        for message in ("The timestamp is not trusted.", "A timestamp was expected but was not found.",
                        "The specified item could not be found in the keychain.", "resource envelope is obsolete"):
            with self.subTest(message=message), \
                    patch.object(sign_macos, "run", return_value=self.result(1, message)) as run, \
                    patch.object(sign_macos.time, "sleep") as sleep, self.assertRaises(RuntimeError):
                sign_macos.sign_with_timestamp(*self.args(), phase="Sign distribution")
            self.assertEqual(run.call_count, 1)
            sleep.assert_not_called()

    def test_generic_command_timeout_is_not_mistaken_for_timestamp_service_failure(self):
        with patch.object(sign_macos, "run", side_effect=sign_macos.CommandTimeout("codesign timed out")) as run, \
                patch.object(sign_macos.time, "sleep") as sleep, self.assertRaises(sign_macos.CommandTimeout):
            sign_macos.sign_with_timestamp(*self.args(), phase="Sign distribution")
        self.assertEqual(run.call_count, 1)
        sleep.assert_not_called()

    def test_exhausted_signing_budget_prevents_retry_sleep(self):
        with patch.object(sign_macos, "run", return_value=self.result(1, UNAVAILABLE)) as run, \
                patch.object(sign_macos, "_signing_deadline", 10), \
                patch.object(sign_macos.time, "monotonic", return_value=10), \
                patch.object(sign_macos.time, "sleep") as sleep, self.assertRaises(sign_macos.SigningTimeout):
            sign_macos.sign_with_timestamp(*self.args(), phase="Sign distribution")
        self.assertEqual(run.call_count, 1)
        sleep.assert_not_called()

    def fixture(self, root):
        app = root / "die.app"
        binaries = app / "Contents/MacOS"
        binaries.mkdir(parents=True)
        for name in ("die", "diec", "die-update"):
            (binaries / name).write_bytes(b"\xfe\xed\xfa\xcf" + b"fake Mach-O")
        framework = app / "Contents/Frameworks/QtConcurrent.framework"
        (framework / "Versions/5").mkdir(parents=True)
        (framework / "Versions/5/QtConcurrent").write_bytes(b"\xfe\xed\xfa\xcf" + b"fake framework")
        (app / "Contents/Info.plist").write_bytes(plistlib.dumps(
            {"CFBundleExecutable": "die", "CFBundleIdentifier": "com.horsicq.die"}))
        return app, framework

    def test_nested_framework_signing_recovers_and_verification_is_not_retried(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            app, framework = self.fixture(root)
            calls = []
            failed_target = framework / "Versions/5/QtConcurrent"

            def run(*args, **kwargs):
                calls.append((args, kwargs))
                if "--verify" in args:
                    raise RuntimeError(UNAVAILABLE)
                if args[-1] == failed_target and sum(call[0][-1] == failed_target for call in calls) == 1:
                    return self.result(1, UNAVAILABLE)
                return self.result(0)

            with patch.dict(os.environ, {"CMAKE_OSX_ARCHITECTURES": "x86_64"}), \
                    patch.object(sign_macos, "run", side_effect=run), \
                    patch.object(sign_macos.time, "sleep") as sleep, self.assertRaisesRegex(RuntimeError, UNAVAILABLE):
                sign_macos.sign_app(app, root, root / "keychain", "fake identity")
            signed = [args for args, _ in calls if "--sign" in args]
            verified = [args for args, _ in calls if "--verify" in args]
            self.assertEqual(sum(args[-1] == failed_target for args in signed), 2)
            for target in (framework, app / "Contents/MacOS/die-update", app / "Contents/MacOS/diec", app):
                self.assertTrue(any(args[-1] == target for args in signed))
            self.assertTrue(all("--timestamp" in args and "--timestamp=none" not in args for args in signed))
            self.assertEqual(len(verified), 1)
            sleep.assert_called_once_with(15)


if __name__ == "__main__":
    unittest.main()
