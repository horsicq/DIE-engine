set VSVARS_PATH="C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\vcvars32.bat"
set QMAKE_PATH="C:\Qt5.6.3\5.6.3\msvc2013\bin\qmake.exe"
set SEVENZIP_PATH="C:\Program Files\7-Zip\7z.exe"

set X_SOURCE_PATH=%~dp0
set X_BUILD_NAME=die_winxp_portable
set /p X_RELEASE_VERSION=<%X_SOURCE_PATH%\release_version.txt

call %X_SOURCE_PATH%\build_tools\windows.cmd check_file %VSVARS_PATH%
call %X_SOURCE_PATH%\build_tools\windows.cmd check_file %QMAKE_PATH%
call %X_SOURCE_PATH%\build_tools\windows.cmd check_file %SEVENZIP_PATH%

IF NOT [%X_ERROR%] == [] goto exit

call %X_SOURCE_PATH%\build_tools\windows.cmd make_init
call %X_SOURCE_PATH%\build_tools\windows.cmd make_build %X_SOURCE_PATH%\die_source.pro

cd %X_SOURCE_PATH%\gui_source
call %X_SOURCE_PATH%\build_tools\windows.cmd make_translate gui_source_tr.pro 
cd %X_SOURCE_PATH%

call %X_SOURCE_PATH%\build_tools\windows.cmd check_file %X_SOURCE_PATH%\build\release\die.exe

IF NOT [%X_ERROR%] == [] goto exit

call %X_SOURCE_PATH%\build_tools\windows.cmd check_file %X_SOURCE_PATH%\build\release\diec.exe

IF NOT [%X_ERROR%] == [] goto exit

mkdir %X_SOURCE_PATH%\release\%X_BUILD_NAME%\signatures

copy %X_SOURCE_PATH%\build\release\die.exe %X_SOURCE_PATH%\release\%X_BUILD_NAME%\
copy %X_SOURCE_PATH%\build\release\diec.exe %X_SOURCE_PATH%\release\%X_BUILD_NAME%\
xcopy %X_SOURCE_PATH%\XStyles\qss %X_SOURCE_PATH%\release\%X_BUILD_NAME%\qss /E /I /Y
xcopy %X_SOURCE_PATH%\Detect-It-Easy\db %X_SOURCE_PATH%\release\%X_BUILD_NAME%\db /E /I /Y
xcopy %X_SOURCE_PATH%\Detect-It-Easy\info %X_SOURCE_PATH%\release\%X_BUILD_NAME%\info /E /I /Y
xcopy %X_SOURCE_PATH%\signatures\crypto.db %X_SOURCE_PATH%\release\%X_BUILD_NAME%\signatures\ /Y

call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_library Qt5Widgets
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_library Qt5Gui
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_library Qt5Core
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_library Qt5OpenGL
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_library Qt5Svg
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_library Qt5Widgets
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_library Qt5Script
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_library Qt5ScriptTools
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_library Qt5Network
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_plugin platforms qwindows
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_plugin imageformats qjpeg
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_plugin imageformats qtiff
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_plugin imageformats qico
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_plugin imageformats qgif
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_vc_redist

call %X_SOURCE_PATH%\build_tools\windows.cmd make_release

:exit
call %X_SOURCE_PATH%\build_tools\windows.cmd make_clear