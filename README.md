[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=NF3FBD3KHMXDN)
[![GitHub tag (latest SemVer)](https://img.shields.io/github/tag/horsicq/DIE-engine.svg)](http://ntinfo.biz)
[![GitHub All Releases](https://img.shields.io/github/downloads/horsicq/DIE-engine/total.svg)](http://ntinfo.biz)
[![gitlocalized ](https://gitlocalize.com/repo/4736/whole_project/badge.svg)](https://gitlocalize.com/repo/4736/whole_project?utm_source=badge)

# DIE-engine
GUI & console sources for Detect It Easy(DiE): https://github.com/horsicq/Detect-It-Easy/

## Updating the signature database

`die-update` checks and installs the database published in the
`current-database` release. It uses the same `Scan/Die/Database/Main/Path` and
`Scan/Die/Database/Main/UpdateUrl` settings as Detect It Easy, leaves
`db_custom` untouched, stages and validates a new `db` before replacing the old
one, and writes `db/die-update.ini` only after the replacement succeeds.

```text
die-update --check             check only (exit 1 when an update is available)
die-update                     install an available update
die-update --help              list paths, sources, JSON and other options
diec --checkupdate [-D path]   run die-update --check and return its exit code
diec --update [-D path]        run die-update and return its exit code
```

The GUI provides **Update database** beside its other main-window actions. The
operation runs in a separate process and the scan database is reloaded after a
successful update. Portable copies read `die.ini` beside the programs; other
copies use the native `NTInfo/die` settings store. Read-only installations put
updates in the per-user database folder, which the Qt frontends prefer on the
next load.

## Special Thanks

- [PELock Software Protection & Reverse Engineering](https://www.pelock.com)

![alt text](https://github.com/horsicq/Detect-It-Easy/blob/master/docs/logo2.png "logo")


# Thanks to all the people who already contributed!

<a href="https://github.com/horsicq/DIE-engine/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=horsicq/DIE-engine" />
</a>


