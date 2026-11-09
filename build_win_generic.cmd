rem %~dp0 ends in "\": strip it, or "%X_SOURCE_PATH%" ends in \" and escapes the closing quote.
if "%X_SOURCE_PATH:~-1%"=="\" set "X_SOURCE_PATH=%X_SOURCE_PATH:~0,-1%"
set "X_BUILD_DIR=%TEMP%\die_cmake_%X_BUILD_PREFIX%"
set "X_INSTALL_DIR=%X_SOURCE_PATH%\release\%X_BUILD_NAME%_%X_BUILD_PREFIX%"
set "X_BUILD_RESULT=0"
if not defined X_WITH_PEID set "X_WITH_PEID=OFF"

if exist "%X_INSTALL_DIR%" rmdir /s /q "%X_INSTALL_DIR%"
if exist "%X_BUILD_DIR%"   rmdir /s /q "%X_BUILD_DIR%"

cmake -S "%X_SOURCE_PATH%" -B "%X_BUILD_DIR%" ^
    -G "Ninja" ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DWITH_PEID=%X_WITH_PEID% ^
    -DCMAKE_PREFIX_PATH="%QT_PREFIX_PATH%"
if errorlevel 1 (set "X_BUILD_RESULT=1" & goto :exit)

cmake --build "%X_BUILD_DIR%"
if errorlevel 1 (set "X_BUILD_RESULT=1" & goto :exit)

cmake --install "%X_BUILD_DIR%" --prefix "%X_INSTALL_DIR%"
if errorlevel 1 (set "X_BUILD_RESULT=1" & goto :exit)


xcopy "%X_SOURCE_PATH%\Detect-It-Easy\db"        "%X_INSTALL_DIR%\db\"          /E /I /Y
for /D %%D in ("%X_SOURCE_PATH%\Detect-It-Easy\db_extra\*") do xcopy "%%D" "%X_INSTALL_DIR%\db\%%~nxD\" /E /I /Y
xcopy "%X_SOURCE_PATH%\XStyles\qss"              "%X_INSTALL_DIR%\qss\"         /E /I /Y
xcopy "%X_SOURCE_PATH%\XInfoDB\info"             "%X_INSTALL_DIR%\info\"        /E /I /Y
xcopy "%X_SOURCE_PATH%\XYara\yara_rules"         "%X_INSTALL_DIR%\yara_rules\"  /E /I /Y
if /I not "%X_WITH_PEID%"=="ON" if exist "%X_INSTALL_DIR%\yara_rules\peid.yar" del /Q "%X_INSTALL_DIR%\yara_rules\peid.yar"
xcopy "%X_SOURCE_PATH%\images"                   "%X_INSTALL_DIR%\images\"      /E /I /Y
if exist "%X_SOURCE_PATH%\signatures\crypto.db" (
    mkdir "%X_INSTALL_DIR%\signatures" 2>nul
    copy  "%X_SOURCE_PATH%\signatures\crypto.db" "%X_INSTALL_DIR%\signatures\"
)

if exist %SEVENZIP_PATH% (
    if exist "%X_SOURCE_PATH%\release\%X_BUILD_NAME%_%X_BUILD_PREFIX%_%X_RELEASE_VERSION%.zip" ^
        del /f /q "%X_SOURCE_PATH%\release\%X_BUILD_NAME%_%X_BUILD_PREFIX%_%X_RELEASE_VERSION%.zip"
    %SEVENZIP_PATH% a -tzip "%X_SOURCE_PATH%\release\%X_BUILD_NAME%_%X_BUILD_PREFIX%_%X_RELEASE_VERSION%.zip" "%X_INSTALL_DIR%\*"
    if /I not "%X_WITH_PEID%"=="ON" %SEVENZIP_PATH% d "%X_SOURCE_PATH%\release\%X_BUILD_NAME%_%X_BUILD_PREFIX%_%X_RELEASE_VERSION%.zip" "*peid*" -r
)

:exit
if exist "%X_BUILD_DIR%" rmdir /s /q "%X_BUILD_DIR%"
exit /b %X_BUILD_RESULT%