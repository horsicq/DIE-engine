"""Exercise macOS packaging with local mock tools, without building or signing."""

import json
import os
from pathlib import Path
import shutil
import stat
import subprocess
import sys
import tempfile
import unittest
import zipfile


SCRIPT = Path(__file__).resolve().parents[3] / "build_mac.sh"
ZIP_NAME = "die_4.0.0_mac_portable.zip"
PKG_NAME = "die_4.0.0_mac.pkg"

MOCK_TOOL = r'''
import json
import os
from pathlib import Path
import stat
import sys
import zipfile

name = Path(sys.argv[0]).name
args = sys.argv[1:]
case = Path(os.environ["TEST_CASE"])
failure = os.environ.get("TEST_FAILURE", "")
with (case / "tools.jsonl").open("a") as log:
    log.write(json.dumps({"tool": name, "args": args, "cwd": os.getcwd()}) + "\n")

def snapshot(app):
    result = {}
    for path in sorted(app.rglob("*")):
        entry = {"mode": stat.S_IMODE(path.lstat().st_mode)}
        if path.is_symlink():
            entry.update(kind="link", data=os.readlink(path))
        elif path.is_dir():
            entry.update(kind="directory")
        else:
            entry.update(kind="file", data=path.read_text())
        result[path.relative_to(app).as_posix()] = entry
    return result

def executable(path, content):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(content)
    path.chmod(0o755)

if name == "cmake":
    if args[0] != "--build":
        if failure == "configure":
            sys.exit(29)
        build = Path(args[args.index("-B") + 1])
        (case / "build-dir").write_text(str(build))
    else:
        if failure == "build":
            sys.exit(29)
        build = Path(args[1])
        app = build / "src/gui" / os.environ.get("TEST_APP", "die.app")
        if failure != "missing-app":
            executable(app / "Contents/MacOS/die", "GUI binary")
            (app / "Contents/Info.plist").write_text("original app identity")
        (build / "src/console").mkdir(parents=True)
        if failure != "missing-diec":
            executable(build / "src/console/diec", "CLI binary")
        updater = os.environ.get("TEST_UPDATER", "absent")
        if updater != "absent":
            (build / "src/updater").mkdir(parents=True)
            if updater == "present":
                executable(build / "src/updater/die-update", "updater binary")
elif name == "sysctl":
    print(2)
elif name == "macdeployqt":
    if failure == "macdeployqt":
        sys.exit(29)
    for arg in args:
        if arg.startswith("-executable="):
            assert Path(arg.split("=", 1)[1]).is_file(), arg
    app = Path(args[0])
    framework = app / "Contents/Frameworks/QtCore.framework"
    executable(framework / "Versions/A/QtCore", "deployed Qt library")
    (framework / "Versions/Current").symlink_to("A", target_is_directory=True)
    (framework / "QtCore").symlink_to("Versions/Current/QtCore")
    (app / "Contents/Resources/deployed.txt").write_text("final deployment")
elif name == "cp":
    if failure == "copy-extra" and any("/db_extra/" in arg for arg in args):
        sys.exit(29)
    if failure == "copy-stage" and args[0] == "-R":
        sys.exit(29)
    os.execv(os.environ["TEST_REAL_CP"], [os.environ["TEST_REAL_CP"], *args])
elif name == "productbuild":
    assert len(args) == 4 and args[0] == "--component", args
    assert args[2] == "/Applications", args
    app, output = Path(args[1]), Path(args[3])
    assert (app / "Contents/MacOS/diec").is_file()
    assert (app / "Contents/Resources/deployed.txt").is_file()
    if failure == "productbuild":
        output.write_text("partial installer")
        sys.exit(29)
    if failure == "missing-pkg":
        sys.exit(0)
    if failure == "empty-pkg":
        output.touch()
    else:
        output.write_text(json.dumps({"component": str(app), "manifest": snapshot(app)}))
elif name == "zip":
    assert args[:2] == ["-y", "-r"], args
    archive, directory = map(Path, args[2:])
    if failure == "zip":
        archive.write_text("partial archive")
        sys.exit(29)
    if failure == "missing-zip":
        sys.exit(0)
    if failure == "empty-zip":
        archive.touch()
    else:
        with zipfile.ZipFile(archive, "w") as output:
            for path in sorted(directory.rglob("*")):
                info = zipfile.ZipInfo(path.as_posix() + ("/" if path.is_dir() and not path.is_symlink() else ""))
                info.create_system = 3
                info.external_attr = path.lstat().st_mode << 16
                if path.is_symlink():
                    data = os.readlink(path).encode()
                elif path.is_dir():
                    data = b""
                else:
                    data = path.read_bytes()
                output.writestr(info, data)
else:
    raise AssertionError(name)
'''


class MacPackagingTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory(prefix="die-mac-build-test-")
        self.addCleanup(temporary.cleanup)
        self.root = Path(temporary.name)
        self.source = self.root / "source tree with spaces"
        self.source.mkdir()
        (self.source / "release_version.txt").write_text("4.0.0\n")
        for directory in (
            "dep/XStyles/qss", "dep/XInfoDB/info", "dep/Detect-It-Easy/db",
            "dep/XYara/yara_rules", "images",
        ):
            path = self.source / directory
            path.mkdir(parents=True)
            (path / "base.txt").write_text("required resource")
        self.bin = self.root / "bin"
        self.bin.mkdir()
        self.mock = self.bin / "mock.py"
        self.mock.write_text(f"#!{sys.executable}\n" + MOCK_TOOL)
        self.mock.chmod(0o755)
        for tool in ("cmake", "sysctl", "cp", "productbuild", "zip"):
            (self.bin / tool).symlink_to(self.mock)
        self.qt = self.root / "Qt with spaces"
        (self.qt / "bin").mkdir(parents=True)
        (self.qt / "bin/macdeployqt").symlink_to(self.mock)
        self.release = self.source / "release"
        self.release.mkdir()
        (self.release / PKG_NAME).write_text("stale installer")

    def run_build(self, *, failure="", updater="absent", app="die.app", extras="absent", qt_argument=False):
        extra = self.source / "dep/Detect-It-Easy/db_extra"
        if extras != "absent":
            (extra / ".vscode").mkdir(parents=True)
            (extra / ".vscode/settings.json").write_text("{}")
            (extra / "about.txt").write_text("metadata")
            if extras == "present":
                (extra / "PE extra").mkdir()
                (extra / "PE extra/rule.sg").write_text("additional rule")
        if failure == "missing-macdeployqt":
            (self.qt / "bin/macdeployqt").unlink()
        env = {
            "PATH": f"{self.bin}{os.pathsep}/usr/bin{os.pathsep}/bin",
            "HOME": str(self.root), "TMPDIR": str(self.root),
            "PYTHONDONTWRITEBYTECODE": "1", "TEST_CASE": str(self.root),
            "TEST_FAILURE": failure, "TEST_UPDATER": updater, "TEST_APP": app,
            "TEST_REAL_CP": shutil.which("cp"),
        }
        arguments = [shutil.which("bash"), str(SCRIPT)]
        if qt_argument:
            arguments.append(str(self.qt))
        else:
            env["QT_PREFIX_PATH"] = str(self.qt)
        result = subprocess.run(arguments, cwd=self.source, env=env, capture_output=True, text=True)
        marker = self.root / "build-dir"
        if marker.exists():
            self.assertFalse(Path(marker.read_text()).exists(), "temporary build output was not cleaned")
        return result

    def events(self):
        return [json.loads(line) for line in (self.root / "tools.jsonl").read_text().splitlines()]

    def assert_outputs(self, *, app="die.app", updater=False, extras=False):
        package = json.loads((self.release / PKG_NAME).read_text())
        app_name = Path(app).name
        staged = self.release / "die_mac_portable" / app_name
        self.assertEqual(package["component"], str(staged))
        self.assertFalse(staged.exists(), "successful build did not remove the staging directory")
        manifest = package["manifest"]
        with zipfile.ZipFile(self.release / ZIP_NAME) as archive:
            prefix = f"die_mac_portable/{app_name}/"
            zip_manifest = {}
            for item in archive.infolist():
                if not item.filename.startswith(prefix) or item.filename == prefix:
                    continue
                key = item.filename[len(prefix):].rstrip("/")
                mode = item.external_attr >> 16
                entry = {"mode": stat.S_IMODE(mode)}
                if stat.S_ISLNK(mode):
                    entry.update(kind="link", data=archive.read(item).decode())
                elif item.is_dir():
                    entry.update(kind="directory")
                else:
                    entry.update(kind="file", data=archive.read(item).decode())
                zip_manifest[key] = entry
        self.assertEqual(zip_manifest, manifest, "ZIP and installer must contain the same deployed app")
        self.assertEqual(manifest["Contents/Info.plist"]["data"], "original app identity")
        self.assertEqual(manifest["Contents/MacOS/diec"]["mode"], 0o755)
        self.assertEqual(manifest["Contents/Frameworks/QtCore.framework/Versions/Current"]["kind"], "link")
        self.assertEqual(manifest["Contents/Frameworks/QtCore.framework/QtCore"]["data"], "Versions/Current/QtCore")
        self.assertIn("Contents/Resources/deployed.txt", manifest)
        self.assertEqual("Contents/MacOS/die-update" in manifest, updater)
        self.assertEqual("Contents/Resources/db/PE extra/rule.sg" in manifest, extras)
        self.assertFalse(any(".vscode" in key or key.endswith("/about.txt") for key in manifest))
        events = self.events()
        deploy = next(event for event in events if event["tool"] == "macdeployqt")
        executables = [arg for arg in deploy["args"] if arg.startswith("-executable=")]
        self.assertEqual(len(executables), 2 if updater else 1)
        self.assertTrue(any(arg.endswith("/diec") for arg in executables))
        self.assertEqual(any(arg.endswith("/die-update") for arg in executables), updater)
        product = next(event for event in events if event["tool"] == "productbuild")
        self.assertEqual(product["args"][:3], ["--component", str(staged), "/Applications"])

    def test_both_packages_preserve_deployed_app_with_optional_inputs_absent(self):
        result = self.run_build()
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assert_outputs()

    def test_both_packages_include_extra_rules_and_updater(self):
        result = self.run_build(extras="present", updater="present", qt_argument=True)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assert_outputs(updater=True, extras=True)

    def test_metadata_only_extra_database_and_empty_updater_are_optional(self):
        result = self.run_build(extras="metadata", updater="empty")
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assert_outputs()

    def test_alternative_bundle_keeps_its_name_in_both_packages(self):
        result = self.run_build(app="DetectItEasy.app")
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assert_outputs(app="DetectItEasy.app")

    def test_release_configuration_bundle_is_supported(self):
        result = self.run_build(app="Release/die.app")
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assert_outputs(app="Release/die.app")

    def test_missing_mandatory_build_outputs_fail_before_packaging(self):
        for failure, diagnostic in (("missing-app", "app bundle not found"), ("missing-diec", "diec not found")):
            with self.subTest(failure=failure):
                result = self.run_build(failure=failure)
                self.assertNotEqual(result.returncode, 0)
                self.assertIn(diagnostic, result.stderr)
                self.assertFalse((self.release / PKG_NAME).exists())
                self.assertFalse((self.release / ZIP_NAME).exists())
                self.assertFalse(any(event["tool"] == "productbuild" for event in self.events()))

    def test_build_and_copy_failures_propagate_and_remove_stale_installer(self):
        for failure in ("configure", "build", "copy-extra", "copy-stage", "macdeployqt", "productbuild", "zip"):
            with self.subTest(failure=failure):
                (self.release / PKG_NAME).write_text("stale installer")
                # Give each run fresh optional resources and a fresh tool log.
                extra = self.source / "dep/Detect-It-Easy/db_extra"
                if extra.exists():
                    shutil.rmtree(extra)
                (self.root / "tools.jsonl").unlink(missing_ok=True)
                result = self.run_build(failure=failure, extras="present")
                self.assertEqual(result.returncode, 29, result.stderr)
                self.assertFalse((self.release / PKG_NAME).exists())
                events = self.events()
                if failure not in ("productbuild", "zip"):
                    self.assertFalse(any(event["tool"] == "productbuild" for event in events))

    def test_missing_deployment_tool_fails(self):
        result = self.run_build(failure="missing-macdeployqt")
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("macdeployqt not found", result.stderr)
        self.assertFalse((self.release / PKG_NAME).exists())

    def test_successful_tool_must_create_nonempty_output(self):
        for failure, tool in (("missing-pkg", "productbuild"), ("empty-pkg", "productbuild"),
                              ("missing-zip", "zip"), ("empty-zip", "zip")):
            with self.subTest(failure=failure):
                (self.release / PKG_NAME).write_text("stale installer")
                result = self.run_build(failure=failure)
                self.assertNotEqual(result.returncode, 0)
                self.assertIn(f"ERROR: {tool} did not create a non-empty", result.stderr)
                self.assertFalse((self.release / PKG_NAME).exists())


if __name__ == "__main__":
    unittest.main()
