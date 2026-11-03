"""Signing failure regressions; Apple tools are fake local executables or mocks."""

import contextlib
import importlib.util
import io
import json
import os
from pathlib import Path
import signal
import subprocess
import sys
import tempfile
import time
import unittest
from unittest.mock import patch


SCRIPT = Path(__file__).resolve().parents[1] / "sign_macos.py"
SPEC = importlib.util.spec_from_file_location("sign_macos_commands", SCRIPT)
sign_macos = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(sign_macos)
SUBMISSION_ID = "11111111-2222-3333-4444-555555555555"


class CommandTestCase(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory(prefix="die-signing-commands-")
        self.addCleanup(temporary.cleanup)
        self.root = Path(temporary.name)
        self.bin = self.root / "bin"
        self.bin.mkdir()
        environment = patch.dict(os.environ, {
            "RUNNER_TEMP": str(self.root),
            "PATH": str(self.bin) + os.pathsep + os.defpath,
        }, clear=True)
        environment.start()
        self.addCleanup(environment.stop)
        deadline = patch.object(sign_macos, "_signing_deadline", None)
        deadline.start()
        self.addCleanup(deadline.stop)

    def executable(self, name, body):
        executable = self.bin / name
        executable.write_text(f"#!{sys.executable}\n" + body)
        executable.chmod(0o700)
        return executable

    def diagnostics(self):
        return [json.loads(path.read_text()) for path in sorted(
            (self.root / "die-apple-notarization/commands").glob("*.json"))]

    def captured(self):
        output = io.StringIO()
        stack = contextlib.ExitStack()
        stack.enter_context(contextlib.redirect_stdout(output))
        stack.enter_context(contextlib.redirect_stderr(output))
        return stack, output


class StaplerRetryTests(CommandTestCase):
    def invoke(self, responses, *, action="staple", kind="app"):
        target = self.root / ("die.pkg" if kind == "installer" else "die.app")
        results = [subprocess.CompletedProcess([], value, "CloudKit response", "")
                   if isinstance(value, int) else value for value in responses]
        stack, output = self.captured()
        error = None
        with stack, patch.object(sign_macos, "run", side_effect=results) as run, \
                patch.object(sign_macos.time, "sleep") as sleep:
            try:
                sign_macos.stapler(action, target, kind=kind)
            except RuntimeError as caught:
                error = caught
        return run, sleep, output.getvalue(), error

    def test_cloudkit_exit_68_recovers_for_app_and_installer_stapling(self):
        for kind in ("app", "installer"):
            with self.subTest(kind=kind):
                run, sleep, output, error = self.invoke([68, 0], kind=kind)
                self.assertIsNone(error)
                self.assertEqual(run.call_count, 2)
                sleep.assert_called_once_with(15)
                self.assertIn("68", output)
                for call in run.call_args_list:
                    self.assertEqual(call.args[:3], ("xcrun", "stapler", "staple"))
                    self.assertFalse(call.kwargs["check"])
                    self.assertLessEqual(call.kwargs["timeout"], 120)

    def test_temporary_failure_and_timeout_recover_during_validation(self):
        run, sleep, _, error = self.invoke(
            [75, sign_macos.CommandTimeout("simulated timeout"), 0], action="validate")
        self.assertIsNone(error)
        self.assertEqual(run.call_count, 3)
        self.assertEqual([call.args[0] for call in sleep.call_args_list], [15, 30])
        self.assertTrue(all(call.args[:3] == ("xcrun", "stapler", "validate")
                            for call in run.call_args_list))

    def test_exhausted_network_retries_fail_without_skipping_validation(self):
        for action in ("staple", "validate"):
            with self.subTest(action=action):
                run, sleep, _, error = self.invoke([68, 68, 68], action=action)
                self.assertIsInstance(error, RuntimeError)
                self.assertIn("68", str(error))
                self.assertEqual(run.call_count, 3)
                self.assertEqual(sleep.call_count, 2)

    def test_exhausted_timeouts_fail_after_three_attempts(self):
        run, sleep, _, error = self.invoke(
            [sign_macos.CommandTimeout("simulated timeout") for _ in range(3)])
        self.assertIsInstance(error, RuntimeError)
        self.assertEqual(run.call_count, 3)
        self.assertEqual(sleep.call_count, 2)

    def test_invalid_ticket_or_other_permanent_errors_are_not_retried(self):
        for code in (1, 65, 66, 77):
            with self.subTest(code=code):
                run, sleep, _, error = self.invoke([code])
                self.assertIsInstance(error, RuntimeError)
                self.assertIn(str(code), str(error))
                self.assertEqual(run.call_count, 1)
                sleep.assert_not_called()


class SubprocessTests(CommandTestCase):
    def test_stapler_stdout_and_stderr_are_retained_and_secrets_are_redacted(self):
        self.executable("xcrun", "import os, sys\n"
                        "print('CloudKit ticket lookup failed')\n"
                        "print(os.environ['APPLE_NOTARY_PASSWORD'])\n"
                        "print('connection lost', file=sys.stderr)\n"
                        "sys.exit(68)\n")
        stack, output = self.captured()
        with stack, patch.dict(os.environ, {"APPLE_NOTARY_PASSWORD": "sensitive-test-password"}):
            with self.assertRaises(RuntimeError) as error:
                sign_macos.run("xcrun", "stapler", "staple", "die.app",
                               phase="Staple app")
        diagnostics = self.diagnostics()
        self.assertEqual(len(diagnostics), 1)
        self.assertEqual(diagnostics[0]["status"], "failed")
        self.assertEqual(diagnostics[0]["returncode"], 68)
        self.assertIn("CloudKit ticket lookup failed", output.getvalue())
        self.assertIn("connection lost", output.getvalue())
        self.assertIn("CloudKit ticket lookup failed", diagnostics[0]["stdout"])
        self.assertIn("connection lost", diagnostics[0]["stderr"])
        combined = output.getvalue() + json.dumps(diagnostics) + str(error.exception)
        self.assertNotIn("sensitive-test-password", combined)

    def test_credentials_and_sensitive_tool_output_are_never_logged(self):
        for tool, arguments in (("security", ("import",)),
                                ("xcrun", ("notarytool", "submit"))):
            with self.subTest(tool=tool):
                self.executable(tool, "import sys\n"
                                "print('secret in stdout')\n"
                                "print('secret in stderr', file=sys.stderr)\n"
                                "sys.exit(1)\n")
                stack, output = self.captured()
                with stack, self.assertRaises(RuntimeError) as error:
                    sign_macos.run(tool, *arguments, "--password", "secret in argument")
                combined = output.getvalue() + json.dumps(self.diagnostics()) + str(error.exception)
                self.assertNotIn("secret in", combined)

    def test_timeout_kills_the_tool_and_its_child_and_retains_partial_output(self):
        marker = self.root / "child-survived"
        self.executable("xcrun", "import os, pathlib, time\n"
                        "print('waiting for CloudKit', flush=True)\n"
                        "if os.fork() == 0:\n"
                        "    time.sleep(1.2)\n"
                        f"    pathlib.Path({str(marker)!r}).write_text('still running')\n"
                        "    os._exit(0)\n"
                        "time.sleep(60)\n")
        started = time.monotonic()
        stack, output = self.captured()
        with stack, self.assertRaises(sign_macos.CommandTimeout):
            sign_macos.run("xcrun", "stapler", "staple", "die.app", timeout=0.25,
                           phase="Staple app")
        self.assertLess(time.monotonic() - started, 3)
        diagnostics = self.diagnostics()
        self.assertEqual(diagnostics[-1]["status"], "timed_out")
        self.assertIn("waiting for CloudKit", json.dumps(diagnostics) + output.getvalue())
        time.sleep(1.3)
        self.assertFalse(marker.exists(), "The timed-out tool left a child running")

    def test_overall_deadline_prevents_starting_another_tool(self):
        marker = self.root / "ran"
        self.executable("xcrun", f"from pathlib import Path\nPath({str(marker)!r}).touch()\n")
        stack, _ = self.captured()
        with stack, patch.object(sign_macos, "_signing_deadline", time.monotonic() - 1), \
                self.assertRaises(RuntimeError):
            sign_macos.run("xcrun", "stapler", "validate", "die.app")
        self.assertFalse(marker.exists())

    def test_overall_deadline_shortens_a_long_tool_timeout(self):
        self.executable("xcrun", "import time\ntime.sleep(60)\n")
        started = time.monotonic()
        stack, _ = self.captured()
        with stack, patch.object(sign_macos, "_signing_deadline", started + 0.25), \
                self.assertRaises(sign_macos.CommandTimeout):
            sign_macos.run("xcrun", "stapler", "staple", "die.app", timeout=120)
        self.assertLess(time.monotonic() - started, 3)
        self.assertLessEqual(self.diagnostics()[-1]["timeout_seconds"], 0.25)

    def test_sigterm_interrupt_kills_active_tool_group(self):
        ready = self.root / "ready"
        marker = self.root / "child-survived-signal"
        self.executable("xcrun", "import os, pathlib, time\n"
                        "if os.fork() == 0:\n"
                        "    time.sleep(1.2)\n"
                        f"    pathlib.Path({str(marker)!r}).touch()\n"
                        "    os._exit(0)\n"
                        f"pathlib.Path({str(ready)!r}).touch()\n"
                        "time.sleep(60)\n")
        driver = self.root / "driver.py"
        driver.write_text(
            "import importlib.util, signal\n"
            f"spec = importlib.util.spec_from_file_location('sign_macos', {str(SCRIPT)!r})\n"
            "module = importlib.util.module_from_spec(spec)\n"
            "spec.loader.exec_module(module)\n"
            "signal.signal(signal.SIGTERM, module.interrupted)\n"
            "try:\n"
            "    module.run('xcrun', 'stapler', 'validate', 'die.app', timeout=30)\n"
            "except RuntimeError as error:\n"
            "    print(error, flush=True)\n")
        with subprocess.Popen([sys.executable, str(driver)], stdout=subprocess.PIPE,
                              stderr=subprocess.PIPE, text=True) as process:
            try:
                end = time.monotonic() + 5
                while not ready.exists() and time.monotonic() < end and process.poll() is None:
                    time.sleep(0.01)
                self.assertTrue(ready.exists(), "Fake stapler never started")
                process.send_signal(signal.SIGTERM)
                stdout, stderr = process.communicate(timeout=5)
            finally:
                if process.poll() is None:
                    process.kill()
                    process.communicate()
        self.assertEqual(process.returncode, 0, stdout + stderr)
        self.assertIn("interrupted", stdout.lower())
        self.assertEqual(self.diagnostics()[-1]["status"], "interrupted")
        time.sleep(1.3)
        self.assertFalse(marker.exists(), "Cancellation left the signing subprocess running")


class NotaryDeadlineTests(CommandTestCase):
    def invoke(self, status, *, log_timeout=False):
        response = json.dumps({"id": SUBMISSION_ID, "status": status})

        def fake_run(*args, **kwargs):
            if args[:3] == ("xcrun", "notarytool", "submit"):
                return subprocess.CompletedProcess(args, 0 if status != "In Progress" else 1,
                                                   response, "sensitive-notary-output")
            if args[:3] == ("xcrun", "notarytool", "log") and log_timeout:
                raise sign_macos.CommandTimeout("Notarization log timed out")
            return subprocess.CompletedProcess(args, 0, "", "")

        stack, output = self.captured()
        error = None
        with stack, patch.dict(os.environ, {
                "APPLE_NOTARY_APPLE_ID": "test@example.invalid",
                "APPLE_NOTARY_PASSWORD": "sensitive-notary-password",
                "APPLE_NOTARY_TEAM_ID": "TESTTEAM"}), \
                patch.object(sign_macos, "run", side_effect=fake_run) as run:
            try:
                sign_macos.notarize_submission(self.root / "submission.zip", self.root / "die.app",
                                               self.root, "app")
            except RuntimeError as caught:
                error = caught
        return run, output.getvalue(), error

    def test_unavailable_log_after_acceptance_does_not_block_stapling(self):
        run, output, error = self.invoke("Accepted", log_timeout=True)
        self.assertIsNone(error)
        self.assertIn("::warning::", output)
        self.assertTrue(any(call.args[:3] == ("xcrun", "stapler", "staple")
                            for call in run.call_args_list))
        submit = next(call for call in run.call_args_list
                      if call.args[:3] == ("xcrun", "notarytool", "submit"))
        log = next(call for call in run.call_args_list
                   if call.args[:3] == ("xcrun", "notarytool", "log"))
        self.assertEqual(submit.kwargs["timeout"], 35 * 60)
        self.assertEqual(log.kwargs["timeout"], 60)
        self.assertNotIn("sensitive-notary", output)

    def test_in_progress_submission_does_not_request_a_log_or_staple(self):
        run, _, error = self.invoke("In Progress")
        self.assertIsInstance(error, RuntimeError)
        self.assertFalse(any(call.args[:3] == ("xcrun", "notarytool", "log") or
                             call.args[:2] == ("xcrun", "stapler")
                             for call in run.call_args_list))

    def test_rejection_remains_fatal_when_diagnostic_log_times_out(self):
        run, _, error = self.invoke("Invalid", log_timeout=True)
        self.assertIsInstance(error, RuntimeError)
        self.assertIn("Apple did not accept", str(error))
        self.assertFalse(any(call.args[:2] == ("xcrun", "stapler")
                             for call in run.call_args_list))


if __name__ == "__main__":
    unittest.main()
