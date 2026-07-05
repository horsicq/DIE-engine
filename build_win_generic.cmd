set "X_BUILD_DIR=%TEMP%\die_cmake_%X_BUILD_PREFIX%"
set "X_INSTALL_DIR=%X_SOURCE_PATH%release\%X_BUILD_NAME%_%X_BUILD_PREFIX%"

if exist "%X_INSTALL_DIR%" rmdir /s /q "%X_INSTALL_DIR%"
if exist "%X_BUILD_DIR%"   rmdir /s /q "%X_BUILD_DIR%"

cmake -S "%X_SOURCE_PATH%" -B "%X_BUILD_DIR%" ^
    -G "NMake Makefiles" ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_PREFIX_PATH="%QT_PREFIX_PATH%"
if errorlevel 1 goto :exit

cmake --build "%X_BUILD_DIR%"
if errorlevel 1 goto :exit

cmake --install "%X_BUILD_DIR%" --prefix "%X_INSTALL_DIR%"
if errorlevel 1 goto :exit

xcopy "%X_SOURCE_PATH%Detect-It-Easy\db"        "%X_INSTALL_DIR%\db\"          /E /I /Y
xcopy "%X_SOURCE_PATH%Detect-It-Easy\db_extra"  "%X_INSTALL_DIR%\db_extra\"    /E /I /Y
xcopy "%X_SOURCE_PATH%XStyles\qss"              "%X_INSTALL_DIR%\qss\"         /E /I /Y
xcopy "%X_SOURCE_PATH%XInfoDB\info"             "%X_INSTALL_DIR%\info\"        /E /I /Y
xcopy "%X_SOURCE_PATH%XYara\yara_rules"         "%X_INSTALL_DIR%\yara_rules\"  /E /I /Y
xcopy "%X_SOURCE_PATH%images"                   "%X_INSTALL_DIR%\images\"      /E /I /Y
if exist "%X_SOURCE_PATH%signatures\crypto.db" (
    mkdir "%X_INSTALL_DIR%\signatures" 2>nul
    copy  "%X_SOURCE_PATH%signatures\crypto.db" "%X_INSTALL_DIR%\signatures\"
)

if exist %SEVENZIP_PATH% (
    if exist "%X_SOURCE_PATH%release\%X_BUILD_NAME%_%X_BUILD_PREFIX%_%X_RELEASE_VERSION%.zip" ^
        del /f /q "%X_SOURCE_PATH%release\%X_BUILD_NAME%_%X_BUILD_PREFIX%_%X_RELEASE_VERSION%.zip"
    %SEVENZIP_PATH% a -tzip "%X_SOURCE_PATH%release\%X_BUILD_NAME%_%X_BUILD_PREFIX%_%X_RELEASE_VERSION%.zip" "%X_INSTALL_DIR%\*"
)

:exit
if exist "%X_BUILD_DIR%" rmdir /s /q "%X_BUILD_DIR%"
