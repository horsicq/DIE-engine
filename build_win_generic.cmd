call %X_SOURCE_PATH%\build_tools\windows.cmd make_init

IF NOT [%X_ERROR%] == [] goto exit

call %X_SOURCE_PATH%\build_tools\windows.cmd make_build %X_SOURCE_PATH%\die_source.pro

cd %X_SOURCE_PATH%\gui_source
call %X_SOURCE_PATH%\build_tools\windows.cmd make_translate gui_source_tr.pro 
cd %X_SOURCE_PATH%
echo "Check files"
rem call %X_SOURCE_PATH%\build_tools\windows.cmd check_file %X_SOURCE_PATH%\build\release\die.exe

IF NOT [%X_ERROR%] == [] goto exit

rem call %X_SOURCE_PATH%\build_tools\windows.cmd check_file %X_SOURCE_PATH%\build\release\diec.exe

IF NOT [%X_ERROR%] == [] goto exit

rem call %X_SOURCE_PATH%\build_tools\windows.cmd check_file %X_SOURCE_PATH%\build\release\diel.exe

IF NOT [%X_ERROR%] == [] goto exit

mkdir %X_SOURCE_PATH%\release\%X_BUILD_NAME%\signatures

copy %X_SOURCE_PATH%\build\release\die.exe %X_SOURCE_PATH%\release\%X_BUILD_NAME%\
copy %X_SOURCE_PATH%\build\release\diec.exe %X_SOURCE_PATH%\release\%X_BUILD_NAME%\
copy %X_SOURCE_PATH%\build\release\diel.exe %X_SOURCE_PATH%\release\%X_BUILD_NAME%\
xcopy %X_SOURCE_PATH%\XStyles\qss %X_SOURCE_PATH%\release\%X_BUILD_NAME%\qss /E /I
xcopy %X_SOURCE_PATH%\Detect-It-Easy\db %X_SOURCE_PATH%\release\%X_BUILD_NAME%\db /E /I
xcopy %X_SOURCE_PATH%\Detect-It-Easy\db_custom %X_SOURCE_PATH%\release\%X_BUILD_NAME%\db_custom /E /I
xcopy %X_SOURCE_PATH%\XInfoDB\info %X_SOURCE_PATH%\release\%X_BUILD_NAME%\info /E /I
xcopy %X_SOURCE_PATH%\signatures\crypto.db %X_SOURCE_PATH%\release\%X_BUILD_NAME%\signatures\
xcopy %X_SOURCE_PATH%\images %X_SOURCE_PATH%\release\%X_BUILD_NAME%\images /E /I
xcopy %X_SOURCE_PATH%\XYara\yara_rules %X_SOURCE_PATH%\release\%X_BUILD_NAME%\yara_rules /E /I

call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_library Qt5Widgets
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_library Qt5Gui
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_library Qt5Core
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_library Qt5OpenGL
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_library Qt5Svg
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_library Qt5Sql
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_library Qt5Script
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_library Qt5ScriptTools
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_library Qt5Network
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_plugin platforms qwindows
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_plugin imageformats qjpeg
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_plugin imageformats qtiff
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_plugin imageformats qico
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_plugin imageformats qgif
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_plugin sqldrivers qsqlite
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_redist
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_openssl

call %X_SOURCE_PATH%\build_tools\windows.cmd make_release

:exit
call %X_SOURCE_PATH%\build_tools\windows.cmd make_clear
