"""Bounded signing diagnostics, using simulated processes and a virtual clock."""

import contextlib
import importlib.util
import io
import json
import os
from pathlib import Path
import subprocess
import tempfile
import unittest
from unittest.mock import Mock, patch


SCRIPT = Path(__file__).resolve().parents[1] / "sign_macos.py"
SPEC = importlib.util.spec_from_file_location("sign_macos_diagnostics", SCRIPT)
sign_macos = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(sign_macos)


class Clock:
    def __init__(self):
        self.now = 1000.0

    def monotonic(self):
        return self.now

    def advance(self, seconds):
        self.now += seconds


class SimulatedProcess:
    """communicate consumes only virtual time; never invokes an external tool."""

    pid = 12345

    def __init__(self, clock, *, completes_after=None):
        self.clock = clock
        self.started = clock.now
        self.completed_at = None if completes_after is None else clock.now + completes_after
        self.returncode = None
        self.waits = []
        self.stopped = False

    def poll(self):
        if self.completed_at is not None and self.clock.now >= self.completed_at:
            self.returncode = 0
        return self.returncode

    def communicate(self, timeout):
        self.waits.append(timeout)
        if self.stopped:
            return "partial hdiutil output", ""
        if self.completed_at is not None and self.clock.now + timeout >= self.completed_at:
            self.clock.now = max(self.clock.now, self.completed_at)
            self.returncode = 0
            return "disk image created", ""
        self.clock.advance(timeout)
        raise subprocess.TimeoutExpired("hdiutil", timeout, output=b"partial hdiutil output")


class DiagnosticSchedulingTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory(prefix="die-signing-diagnostics-")
        self.addCleanup(temporary.cleanup)
        self.root = Path(temporary.name)
        self.clock = Clock()
        self.output = io.StringIO()
        self.stack = contextlib.ExitStack()
        self.addCleanup(self.stack.close)
        self.stack.enter_context(patch.dict(os.environ, {"RUNNER_TEMP": str(self.root)}, clear=True))
        self.stack.enter_context(patch.object(sign_macos.time, "monotonic", self.clock.monotonic))
        self.stack.enter_context(patch.object(sign_macos, "_signing_deadline", None))
        self.stack.enter_context(contextlib.redirect_stdout(self.output))
        self.stack.enter_context(contextlib.redirect_stderr(self.output))

    def run_process(self, process, *, capture=None, timeout=900, thresholds=(60, 300, 840)):
        captures = []
        stopped = []

        def capture_process(child, directory, *, timeout):
            self.assertIs(child, process)
            self.assertIsNone(child.poll(), "Sampling must happen before the process stops")
            captures.append((self.clock.now - child.started, timeout, Path(directory)))
            if capture is not None:
                capture(child, Path(directory), timeout)

        def stop_process(child):
            stopped.append(self.clock.now - child.started)
            child.stopped = True
            child.returncode = -9
            return "partial hdiutil output", ""

        result = error = None
        with patch.object(sign_macos.subprocess, "Popen", return_value=process), \
                patch.object(sign_macos, "capture_process_diagnostics", side_effect=capture_process), \
                patch.object(sign_macos, "stop_process", side_effect=stop_process):
            try:
                result = sign_macos.run("hdiutil", "create", "-srcfolder", "fake contents", "die.dmg",
                                        timeout=timeout, phase="dmg: create compressed disk image",
                                        diagnose_after=thresholds)
            except RuntimeError as caught:
                error = caught
        return result, error, captures, stopped

    def record(self):
        records = list((self.root / "die-apple-notarization/commands").glob("*.json"))
        self.assertEqual(len(records), 1)
        return json.loads(records[0].read_text())

    def test_early_success_does_not_run_diagnostic_tools(self):
        process = SimulatedProcess(self.clock, completes_after=10)
        result, error, captures, stopped = self.run_process(process)
        self.assertIsNone(error)
        self.assertEqual(result.returncode, 0)
        self.assertEqual(captures, [])
        self.assertEqual(stopped, [])
        self.assertEqual(self.record()["status"], "succeeded")

    def test_hdiutil_can_finish_after_old_five_minute_limit(self):
        process = SimulatedProcess(self.clock, completes_after=650)
        result, error, captures, stopped = self.run_process(process)
        self.assertIsNone(error)
        self.assertEqual(result.returncode, 0)
        self.assertEqual([elapsed for elapsed, _, _ in captures], [60, 300])
        self.assertEqual(stopped, [])
        self.assertEqual(self.record()["elapsed_seconds"], 650)
        self.assertEqual(self.record()["timeout_seconds"], 900)

    def test_samples_are_taken_before_kill_without_extending_command_deadline(self):
        process = SimulatedProcess(self.clock)

        def capture(child, directory, timeout):
            self.assertLessEqual(timeout, 20)
            self.clock.advance(8)

        _, error, captures, stopped = self.run_process(process, capture=capture)
        self.assertIsInstance(error, sign_macos.CommandTimeout)
        self.assertEqual([elapsed for elapsed, _, _ in captures], [60, 300, 840])
        self.assertEqual(stopped, [900])
        self.assertEqual(self.record()["status"], "timed_out")
        self.assertEqual(self.record()["elapsed_seconds"], 900)

    def test_unavailable_diagnostics_do_not_fail_successful_hdiutil(self):
        for failure in (OSError("diagnostic tool unavailable"), RuntimeError("sample timed out")):
            with self.subTest(failure=type(failure).__name__):
                process = SimulatedProcess(self.clock, completes_after=100)

                def capture(child, directory, timeout):
                    raise failure

                result, error, captures, stopped = self.run_process(process, capture=capture)
                self.assertIsNone(error)
                self.assertEqual(result.returncode, 0)
                self.assertEqual(len(captures), 1)
                self.assertEqual(stopped, [])
        self.assertIn("::warning::", self.output.getvalue())

    def test_total_signing_deadline_limits_diagnostics_and_waits(self):
        process = SimulatedProcess(self.clock)

        def capture(child, directory, timeout):
            self.assertLessEqual(timeout, 10)
            self.clock.advance(timeout)

        with patch.object(sign_macos, "_signing_deadline", self.clock.now + 70):
            _, error, captures, stopped = self.run_process(process, capture=capture)
        self.assertIsInstance(error, (sign_macos.CommandTimeout, sign_macos.SigningTimeout))
        self.assertEqual(len(captures), 1)
        self.assertEqual(stopped, [70])
        self.assertLessEqual(self.record()["timeout_seconds"], 70)

    def test_cancellation_during_sampling_still_stops_hdiutil(self):
        process = SimulatedProcess(self.clock)

        def capture(child, directory, timeout):
            raise KeyboardInterrupt

        stopped = []

        def stop(child):
            stopped.append(child.pid)
            child.returncode = -9
            return "", ""

        with patch.object(sign_macos.subprocess, "Popen", return_value=process), \
                patch.object(sign_macos, "capture_process_diagnostics", side_effect=capture), \
                patch.object(sign_macos, "stop_process", side_effect=stop), self.assertRaises(KeyboardInterrupt):
            sign_macos.run("hdiutil", "die.dmg", timeout=900, diagnose_after=(60,))
        self.assertEqual(stopped, [process.pid])
        self.assertEqual(self.record()["status"], "interrupted")

    def test_signal_cancellation_is_not_mistaken_for_a_diagnostic_failure(self):
        process = SimulatedProcess(self.clock)

        def capture(child, directory, timeout):
            raise sign_macos.SigningInterrupted("Signing was interrupted")

        _, error, captures, stopped = self.run_process(process, capture=capture)
        self.assertIsInstance(error, sign_macos.SigningInterrupted)
        self.assertEqual(len(captures), 1)
        self.assertEqual(stopped, [60])
        self.assertEqual(self.record()["status"], "interrupted")


class ProcessCollectorTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory(prefix="die-signing-collector-")
        self.addCleanup(temporary.cleanup)
        self.root = Path(temporary.name)
        self.reports = self.root / "die-apple-notarization" / "process-sample"
        self.process = Mock(pid=12345)
        self.process.poll.return_value = None
        self.clock = Clock()
        self.calls = []
        self.sample_files = []
        self.output = io.StringIO()
        self.stack = contextlib.ExitStack()
        self.addCleanup(self.stack.close)
        self.stack.enter_context(patch.dict(os.environ, {
            "RUNNER_TEMP": str(self.root),
            "APPLE_NOTARY_PASSWORD": "private-notary-password",
            "APPLE_CERTIFICATE_PASSWORD": "private-certificate-password",
            "APPLE_NOTARY_KEY_P8": "private-api-key",
        }, clear=True))
        self.stack.enter_context(patch.object(sign_macos.time, "monotonic", self.clock.monotonic))
        self.stack.enter_context(patch.object(sign_macos.sys, "platform", "darwin"))
        self.stack.enter_context(patch.object(sign_macos, "_signing_deadline", None))
        self.stack.enter_context(contextlib.redirect_stdout(self.output))
        self.stack.enter_context(contextlib.redirect_stderr(self.output))

    def fake_run(self, *args, **kwargs):
        self.calls.append((args, kwargs))
        self.assertGreater(kwargs["timeout"], 0)
        self.assertLessEqual(kwargs["timeout"], 20)
        self.assertFalse(kwargs.get("diagnose_after", ()), "Diagnostics must not recursively sample diagnostics")
        self.assertFalse(any(key.startswith("APPLE_") for key in kwargs["env"]))
        if args[0] == "/bin/ps":
            self.assertEqual(args[1:4], ("-g", "12345", "-x"))
            columns = args[args.index("-o") + 1]
            self.assertEqual(columns, "pid=,ppid=,pgid=,stat=,etime=,time=,comm=")
            self.assertNotIn("args", columns)
            self.assertNotIn("command", columns)
            self.assertNotIn("-e", args)
            return subprocess.CompletedProcess(args, 0,
                "12345 100 12345 S 01:00 0:00 /usr/bin/hdiutil\nprivate-notary-password\n", "")
        self.assertEqual(args[0], "/usr/bin/sample")
        self.assertEqual(args[1], "12345")
        self.assertNotIn("sudo", args)
        report = Path(args[args.index("-file") + 1])
        self.assertNotIn(self.reports, report.parents)
        self.sample_files.append(report)
        report.write_text("Call graph: SecKeyCreateSignature\nprivate-certificate-password\nprivate-api-key\n")
        return subprocess.CompletedProcess(args, 0, "sample completed", "private-notary-password")

    def artifact_text(self):
        return "\n".join(path.read_text() for path in self.reports.rglob("*") if path.is_file())

    def test_collector_records_only_redacted_reports_and_no_raw_sample(self):
        with patch.object(sign_macos, "run", side_effect=self.fake_run):
            sign_macos.capture_process_diagnostics(self.process, self.reports, timeout=20)
        self.assertEqual([args[0] for args, _ in self.calls], ["/bin/ps", "/usr/bin/sample"])
        self.assertTrue((self.reports / "process.json").is_file())
        self.assertIn("hdiutil", (self.reports / "ps.txt").read_text())
        self.assertIn("SecKeyCreateSignature", (self.reports / "sample.txt").read_text())
        combined = self.artifact_text() + self.output.getvalue()
        for secret in ("private-notary-password", "private-certificate-password", "private-api-key"):
            self.assertNotIn(secret, combined)
        self.assertTrue(self.sample_files)
        self.assertTrue(all(not path.exists() for path in self.sample_files))

    def test_sample_timeout_keeps_process_snapshot_and_is_nonfatal(self):
        def fake_run(*args, **kwargs):
            if args[0] == "/usr/bin/sample":
                self.calls.append((args, kwargs))
                report = Path(args[args.index("-file") + 1])
                self.sample_files.append(report)
                report.write_text("Partial stack: SecKeyCreateSignature\nprivate-api-key\n")
                raise sign_macos.CommandTimeout("sample timed out")
            return self.fake_run(*args, **kwargs)

        with patch.object(sign_macos, "run", side_effect=fake_run):
            sign_macos.capture_process_diagnostics(self.process, self.reports, timeout=20)
        self.assertTrue((self.reports / "ps.txt").is_file())
        metadata = json.loads((self.reports / "process.json").read_text())
        self.assertNotEqual(metadata["status"], "captured")
        self.assertIn("timed", json.dumps(metadata).lower())
        self.assertIn("SecKeyCreateSignature", (self.reports / "sample.txt").read_text())
        self.assertNotIn("private-api-key", self.artifact_text() + self.output.getvalue())
        self.assertTrue(all(not path.exists() for path in self.sample_files))

    def test_collector_never_extends_the_budget_for_a_second_tool(self):
        def fake_run(*args, **kwargs):
            result = self.fake_run(*args, **kwargs)
            self.clock.advance(kwargs["timeout"])
            return result

        with patch.object(sign_macos, "run", side_effect=fake_run):
            sign_macos.capture_process_diagnostics(self.process, self.reports, timeout=3)
        self.assertEqual(len(self.calls), 1)
        self.assertLessEqual(self.calls[0][1]["timeout"], 3)
        self.assertLessEqual(self.clock.now - 1000, 3)
        self.assertTrue((self.reports / "process.json").exists())

    def test_cancellation_propagates_out_of_collector(self):
        with patch.object(sign_macos, "run", side_effect=sign_macos.SigningInterrupted("Cancelled")), \
                self.assertRaises(sign_macos.SigningInterrupted):
            sign_macos.capture_process_diagnostics(self.process, self.reports, timeout=20)


if __name__ == "__main__":
    unittest.main()
