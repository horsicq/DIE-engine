DIE-engine
==========

DIE engine

How to build:
==========
## Windows:

* Install VS2010: http://msdn.microsoft.com/en-us/library/dd831853%28v=vs.100%29.aspx
* Install QT 4.8.7 http://download.qt.io/archive/qt/4.8/4.8.7/qt-opensource-windows-x86-vs2010-4.8.7.exe
* Download Jom: http://download.qt-project.org/official_releases/jom/jom.zip and unpack to c:\QT\jom

* run build_console_win.bat
* run build_diedll_win.bat
* run build_dielib_win.bat

## Linux:

* Install QT 4.8.7: http://download.qt.io/archive/qt/4.8/4.8.7/qt-everywhere-opensource-src-4.8.7.tar.gz
* Edit build_console_lin.sh: Set PATH for QMAKE

* run build_console_lin.sh ( bash -x build_console_lin.sh )

## Mac:

* install XCODE: https://developer.apple.com/xcode/downloads/
* Install QT 4.8.7: http://download.qt.io/archive/qt/4.8/4.8.7/qt-opensource-mac-4.8.7.dmg

* run build_console_mac.sh ( bash -x build_console_mac.sh )


Don't forget after compilation to get signatures from https://github.com/horsicq/Detect-It-Easy (folder "db").
Just place the signatures to project's folder.
