# Standalone updater integration tests

Build only the updater and run the suite from the repository root:

```sh
cmake -S . -B <build> -G Ninja -DDIE_UPDATER_ONLY=ON
cmake --build <build>
python3 src/updater/tests/test_updater.py --updater <build>/src/updater/die-update
```

On Windows, pass the path to `die-update.exe`. The suite uses Python's standard
library and needs no additional source checkout, fixture corpus, or Python packages. It creates
minimal signature archives and a local HTTP server on `127.0.0.1`; no internet
service is contacted.

Each test isolates the database, settings, and `DIE_USER_DATA_DIR` in a temporary
directory. Commands disable proxies and normally disable user-folder fallback;
the fallback tests enable it only with temporary database paths. They use either
`--nosettings` or an explicit temporary settings file. Existing user settings and
databases are not modified.

Coverage includes direct ZIP/MD5 downloads, GitHub release metadata and redirects,
rate-limit fallback, CLI exit codes and JSON output, check/update/force behavior,
database replacement, preservation of custom and extra databases, malformed ZIPs,
CRC and checksum failures, path traversal, settings precedence, per-user database
selection, read-only installation fallback, credential redaction, and
inactive-download timeout. A POSIX test also interrupts an active
download with SIGTERM and checks database preservation; this case is skipped on
Windows. Real service
availability and platform-specific filesystem locking need separate checks.

Run one case by appending its unittest name:

```sh
python3 src/updater/tests/test_updater.py --updater <build>/src/updater/die-update UpdaterTests.test_direct_check_default_update_and_check
```
