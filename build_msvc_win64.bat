set VSVARS_PATH="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
set QMAKE_PATH="C:\Qt\5.15.2\msvc2019_64\bin\qmake.exe"
set SEVENZIP_PATH="C:\Program Files\7-Zip\7z.exe"
set INNOSETUP_PATH="C:\Program Files (x86)\Inno Setup 6\ISCC.exe"

set X_SOURCE_PATH=%~dp0
set X_BUILD_NAME=die
set X_BUILD_PREFIX=win64
set /p X_RELEASE_VERSION=<%X_SOURCE_PATH%\release_version.txt

call %X_SOURCE_PATH%\build_win_generic_check.cmd
