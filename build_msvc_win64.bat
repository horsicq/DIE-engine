@echo off
setlocal enabledelayedexpansion

set QT_BASE_PATH="C:\Qt"

set VSVARS_PATH=
for /f "tokens=*" %%a in (
    'reg query "HKLM\SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Uninstall" /s /f "Visual Studio" ^| findstr "HKEY"'
) do (
    for /f "tokens=2*" %%b in (
        'reg query "%%a" /v InstallLocation 2^>nul ^| findstr /i "InstallLocation"'
    ) do (
        if not "%%c"=="" (
            if exist "%%c\VC\Auxiliary\Build\vcvars64.bat" (
                set VSVARS_PATH="%%c\VC\Auxiliary\Build\vcvars64.bat"
                goto :found_vs
            )
        )
    )
)

:found_vs
if not defined VSVARS_PATH (
    echo Visual Studio not found.
    goto :exit
)

call %VSVARS_PATH%

set QT_PREFIX_PATH=
for /D %%Q in (%QT_BASE_PATH%\5.*) do (
    for /D %%K in (%%Q\msvc*_64 %%Q\msvc*_arm64) do (
        if exist "%%K\bin\qmake.exe" (
            set QT_PREFIX_PATH=%%K
            goto :found_qt
        )
    )
)

:found_qt
if not defined QT_PREFIX_PATH (
    echo Qt 5.x not found under %QT_BASE_PATH%.
    goto :exit
)

set SEVENZIP_PATH="C:\Program Files\7-Zip\7z.exe"

set X_SOURCE_PATH=%~dp0
set X_BUILD_NAME=die
set X_BUILD_PREFIX=win64
set /p X_RELEASE_VERSION=<%X_SOURCE_PATH%\release_version.txt

call %X_SOURCE_PATH%\build_win_generic_check.cmd

:exit
endlocal
