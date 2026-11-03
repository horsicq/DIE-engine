"""Exercise the temporary PF workaround with fake pfctl processes only."""

import contextlib
import importlib.util
import io
import json
import os
from pathlib import Path
import subprocess
import tempfile
import unittest
from unittest.mock import patch


SCRIPT = Path(__file__).resolve().parents[1] / "stapler_network.py"
SPEC = importlib.util.spec_from_file_location("stapler_network", SCRIPT)
network = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(network)


class NetworkTests(unittest.TestCase):
    architecture = "x86_64"

    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory(prefix="die-stapler-network-test-")
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name)
        self.calls = []
        self.rules = 'scrub-anchor "com.apple/*" all\nanchor "com.apple/*" all\n'
        self.token_output = "pf enabled\nToken : 123456789\n"
        self.failure = None
        self.stack = contextlib.ExitStack()
        self.addCleanup(self.stack.close)
        self.stack.enter_context(patch.dict(os.environ, {
            "RUNNER_TEMP": str(self.root), "RUNNER_ENVIRONMENT": "github-hosted",
            "GITHUB_ACTIONS": "true",
        }, clear=True))
        self.stack.enter_context(patch.object(network.sys, "platform", "darwin"))
        self.stack.enter_context(patch.object(network.platform, "machine", return_value=self.architecture))
        self.stack.enter_context(patch.object(network.subprocess, "run", side_effect=self.fake_run))
        self.stack.enter_context(contextlib.redirect_stdout(io.StringIO()))

    def fake_run(self, command, **kwargs):
        self.calls.append((command, kwargs))
        self.assertEqual(command[:3], ["sudo", "-n", "/sbin/pfctl"])
        self.assertEqual(kwargs["timeout"], 20)
        args = command[3:]
        output = self.rules if args == ["-s", "rules"] else ""
        if args == ["-E"]:
            output = self.token_output
            kwargs["stdout"].write(output)
            kwargs["stdout"].flush()
        if self.failure is not None:
            self.failure(args)
        return subprocess.CompletedProcess(command, 0, output, "")

    def commands(self):
        return [call[0][3:] for call in self.calls]

    def state(self):
        return json.loads((network.state_dir() / "state.json").read_text())

    def test_enable_and_cleanup_only_modify_owned_anchor_and_reference(self):
        network.enable()
        anchor = self.state()["anchor"]
        self.assertTrue(anchor.startswith("com.apple/die-stapler-"))
        self.assertEqual((network.state_dir() / "rules.conf").read_text(),
                         "block return out quick proto udp from any to any port 443\n")
        self.assertEqual(self.commands(), [
            ["-s", "rules"], ["-a", anchor, "-f", str(network.state_dir() / "rules.conf")], ["-E"],
        ])
        network.cleanup()
        self.assertEqual(self.commands()[-2:], [["-a", anchor, "-F", "rules"], ["-X", "123456789"]])
        self.assertFalse(network.state_dir().exists())
        network.cleanup()
        self.assertEqual(len(self.commands()), 5)

    def test_unsupported_runners_are_rejected_before_any_pf_command(self):
        for changes in ({"RUNNER_ENVIRONMENT": "self-hosted"}, {"GITHUB_ACTIONS": "false"}):
            with self.subTest(changes=changes), patch.dict(os.environ, changes), self.assertRaises(RuntimeError):
                network.enable()
        with patch.object(network.platform, "machine", return_value="i386"), self.assertRaises(RuntimeError):
            network.enable()
        with patch.object(network.sys, "platform", "linux"), self.assertRaises(RuntimeError):
            network.enable()
        self.assertEqual(self.calls, [])

    def test_missing_active_parent_anchor_fails_without_reloading_main_rules(self):
        for rules in ('anchor "other/*" all\n', 'scrub-anchor "com.apple/*" all\n'):
            self.rules = rules
            with self.subTest(rules=rules), self.assertRaisesRegex(RuntimeError, "no com.apple/\\* anchor"):
                network.enable()
            self.assertEqual(self.commands()[-1], ["-s", "rules"])
            network.cleanup()
        self.assertFalse(any(command[0] in ("-f", "-E", "-d") for command in self.commands()))

    def test_failed_rule_install_keeps_state_for_cleanup(self):
        def fail(args):
            if "-f" in args:
                raise subprocess.TimeoutExpired("pfctl", 20)
        self.failure = fail
        with self.assertRaisesRegex(RuntimeError, "timed out after 20 seconds"):
            network.enable()
        anchor = self.state()["anchor"]
        self.failure = None
        network.cleanup()
        self.assertEqual(self.commands()[-1], ["-a", anchor, "-F", "rules"])
        self.assertNotIn(["-E"], self.commands())
        self.assertFalse(network.state_dir().exists())

    def test_enable_timeout_preserves_emitted_token_for_later_cleanup(self):
        def fail(args):
            if args == ["-E"]:
                raise subprocess.TimeoutExpired("pfctl", 20)
        self.failure = fail
        with self.assertRaisesRegex(RuntimeError, "timed out"):
            network.enable()
        self.failure = None
        network.cleanup()
        self.assertEqual(self.commands()[-1], ["-X", "123456789"])
        self.assertFalse(network.state_dir().exists())

    def test_missing_enable_token_fails_and_never_disables_global_pf(self):
        self.token_output = "unexpected pfctl output\n"
        with self.assertRaisesRegex(RuntimeError, "did not return an enable token"):
            network.enable()
        with self.assertRaisesRegex(RuntimeError, "cannot safely release"):
            network.cleanup()
        self.assertTrue(network.state_dir().exists())
        self.assertFalse(any(command[0] in ("-X", "-d") for command in self.commands()))

    def test_cleanup_releases_reference_even_if_anchor_removal_times_out(self):
        network.enable()
        def fail(args):
            if "-F" in args:
                raise subprocess.TimeoutExpired("pfctl", 20)
        self.failure = fail
        with self.assertRaisesRegex(RuntimeError, "timed out"):
            network.cleanup()
        self.assertIn(["-X", "123456789"], self.commands())
        self.assertFalse(self.state()["enable_started"])
        self.failure = None
        network.cleanup()
        self.assertEqual(self.commands().count(["-X", "123456789"]), 1)
        self.assertFalse(network.state_dir().exists())

    def test_cleanup_retries_failed_reference_release_without_global_disable(self):
        network.enable()
        def fail(args):
            if args[0] == "-X":
                raise subprocess.TimeoutExpired("pfctl", 20)
        self.failure = fail
        with self.assertRaises(RuntimeError):
            network.cleanup()
        self.assertTrue(self.state()["enable_started"])
        self.failure = None
        network.cleanup()
        self.assertFalse(network.state_dir().exists())

    def test_invalid_anchor_state_cannot_flush_other_services(self):
        network.enable()
        state = self.state()
        state["anchor"] = "com.apple/*"
        network.save_state(network.state_dir(), state)
        count = len(self.calls)
        with self.assertRaisesRegex(RuntimeError, "Invalid stapler PF anchor"):
            network.cleanup()
        self.assertEqual(len(self.calls), count)

    def test_repeated_enable_refuses_to_replace_pending_cleanup_state(self):
        network.enable()
        state = self.state()
        count = len(self.calls)
        with self.assertRaises(FileExistsError):
            network.enable()
        self.assertEqual(self.state(), state)
        self.assertEqual(len(self.calls), count)

    def test_state_directory_without_state_means_no_pf_mutations(self):
        network.state_dir().mkdir()
        network.cleanup()
        self.assertFalse(network.state_dir().exists())
        self.assertEqual(self.calls, [])


class Arm64NetworkTests(NetworkTests):
    """Exercise every enable, failure-recovery, and cleanup path on Apple Silicon."""

    architecture = "arm64"


if __name__ == "__main__":
    unittest.main()
