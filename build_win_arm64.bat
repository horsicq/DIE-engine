@echo off
setlocal EnableDelayedExpansion

if "%X_SOURCE_PATH%"=="" set "X_SOURCE_PATH=%~dp0"
if "%X_SOURCE_PATH:~-1%"=="\" set "X_SOURCE_PATH=%X_SOURCE_PATH:~0,-1%"

echo [*] X_SOURCE_PATH: %X_SOURCE_PATH%
echo [*] QT_HOST_PATH: %QT_HOST_PATH%
echo [*] QT_TARGET_PATH: %QT_TARGET_PATH%

echo [*] Patching xsimd for ARM64 architecture...
if exist "%X_SOURCE_PATH%\dep\Formats\xsimd\src\xsimd_sse2.c" (
    type nul > "%X_SOURCE_PATH%\dep\Formats\xsimd\src\xsimd_sse2.c"
    echo    -^> xsimd_sse2.c has been successfully nulled.
) else (
    echo    -^> WARNING: xsimd_sse2.c not found!
)

set "SEVENZIP_PATH=C:\Program Files\7-Zip\7z.exe"
set "X_BUILD_NAME=die"
set "X_BUILD_PREFIX=winarm64_qt6"

for /f "delims=" %%x in ('type "%X_SOURCE_PATH%\release_version.txt"') do set "X_RELEASE_VERSION=%%x"
echo [*] Build Version: %X_RELEASE_VERSION%

set "X_BUILD_DIR=%X_SOURCE_PATH%\build"
set "X_INSTALL_DIR=%X_SOURCE_PATH%\release\%X_BUILD_NAME%_%X_BUILD_PREFIX%"

if exist "%X_INSTALL_DIR%" rmdir /s /q "%X_INSTALL_DIR%"
if exist "%X_BUILD_DIR%" rmdir /s /q "%X_BUILD_DIR%"

cmake -S "%X_SOURCE_PATH%" -B "%X_BUILD_DIR%" -G "Visual Studio 17 2022" -A ARM64 -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="%QT_TARGET_PATH%" -DQT_HOST_PATH="%QT_HOST_PATH%"
if errorlevel 1 exit /b 1

cmake --build "%X_BUILD_DIR%" --config Release -j %NUMBER_OF_PROCESSORS%
if errorlevel 1 exit /b 1

cmake --install "%X_BUILD_DIR%" --config Release --prefix "%X_INSTALL_DIR%"
if errorlevel 1 exit /b 1

copy /Y "%QT_HOST_PATH%\bin\qtpaths.exe" "%QT_TARGET_PATH%\bin\qtpaths.exe"
if exist "%QT_HOST_PATH%\bin\qtpaths6.exe" copy /Y "%QT_HOST_PATH%\bin\qtpaths6.exe" "%QT_TARGET_PATH%\bin\qtpaths6.exe"

echo [*] Running windeployqt...
for /R "%X_INSTALL_DIR%" %%f in (*.exe) do (
    "%QT_HOST_PATH%\bin\windeployqt.exe" --qtpaths "%QT_TARGET_PATH%\bin\qtpaths.exe" --release "%%f"
)

for %%D in (Qt6Core.dll Qt6Gui.dll Qt6Widgets.dll Qt6Network.dll Qt6Core5Compat.dll Qt6Svg.dll Qt6OpenGL.dll Qt6OpenGLWidgets.dll) do (
    if not exist "%X_INSTALL_DIR%\%%D" (
        if exist "%QT_TARGET_PATH%\bin\%%D" copy /Y "%QT_TARGET_PATH%\bin\%%D" "%X_INSTALL_DIR%\"
    )
)
if not exist "%X_INSTALL_DIR%\platforms" mkdir "%X_INSTALL_DIR%\platforms"
if not exist "%X_INSTALL_DIR%\platforms\qwindows.dll" (
    copy /Y "%QT_TARGET_PATH%\plugins\platforms\qwindows.dll" "%X_INSTALL_DIR%\platforms\"
)

echo [*] Copying DIE Assets (DBs, QSS, Yara, etc.)...
xcopy "%X_SOURCE_PATH%\Detect-It-Easy\db"        "%X_INSTALL_DIR%\db\"          /E /I /Y >nul
xcopy "%X_SOURCE_PATH%\Detect-It-Easy\db_extra"  "%X_INSTALL_DIR%\db_extra\"    /E /I /Y >nul
xcopy "%X_SOURCE_PATH%\XStyles\qss"              "%X_INSTALL_DIR%\qss\"         /E /I /Y >nul
xcopy "%X_SOURCE_PATH%\XInfoDB\info"             "%X_INSTALL_DIR%\info\"        /E /I /Y >nul
xcopy "%X_SOURCE_PATH%\XYara\yara_rules"         "%X_INSTALL_DIR%\yara_rules\"  /E /I /Y >nul
xcopy "%X_SOURCE_PATH%\images"                   "%X_INSTALL_DIR%\images\"      /E /I /Y >nul

if exist "%X_SOURCE_PATH%\signatures\crypto.db" (
    mkdir "%X_INSTALL_DIR%\signatures" 2>nul
    copy  "%X_SOURCE_PATH%\signatures\crypto.db" "%X_INSTALL_DIR%\signatures\" >nul
)

echo [*] Creating Release ZIP...
if exist "%SEVENZIP_PATH%" (
    if exist "%X_SOURCE_PATH%\release\%X_BUILD_NAME%_%X_BUILD_PREFIX%_%X_RELEASE_VERSION%.zip" (
        del /f /q "%X_SOURCE_PATH%\release\%X_BUILD_NAME%_%X_BUILD_PREFIX%_%X_RELEASE_VERSION%.zip"
    )
    "%SEVENZIP_PATH%" a -tzip "%X_SOURCE_PATH%\release\%X_BUILD_NAME%_%X_BUILD_PREFIX%_%X_RELEASE_VERSION%.zip" "%X_INSTALL_DIR%\*"
    if errorlevel 1 exit /b 1
) else (
    echo [ERROR] 7-Zip not found at %SEVENZIP_PATH%
    exit /b 1
)