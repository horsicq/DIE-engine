set VS_PATH="C:\Program Files (x86)\Microsoft Visual Studio 12.0"
set QT_PATH="C:\Qt\5.6.3\msvc2013"
rem set QT_PATH="C:\Qt\Qt5.6.3_msvc2013_x86\5.6.3\msvc2013"
set UPX_PATH="C:\Utils\upx-3.95-win32"
set SEVENZIP_PATH="C:\Program Files\7-Zip"
set CUR_DRIVE=%CD:~0,2%
set SOURCE_PATH=%~dp0
set /p RELEASE_VERSION=<release_version.txt
set BUILD_NAME=die_win32_portable
set BUILD_NAME_DLL=die_win32_dll
set QMAKE=%QT_PATH%\bin\qmake.exe
set LUPDATE=%QT_PATH%\bin\lupdate.exe

set QT_SPEC=win32-msvc2013
call %VS_PATH%\VC\bin\vcvars32.bat

%CUR_DRIVE%
cd %SOURCE_PATH%

rmdir /s /q %SOURCE_PATH%\die_source\build
mkdir %SOURCE_PATH%\die_source\build

cd %SOURCE_PATH%\die_source\gui_source
%QT_PATH%\bin\qmake.exe gui_source.pro -r -spec %QT_SPEC% "CONFIG+=release"

nmake Makefile.Release clean
nmake
del Makefile
del Makefile.Release
del Makefile.Debug

cd %SOURCE_PATH%

cd %SOURCE_PATH%\die_source\console_source
%QT_PATH%\bin\qmake.exe console_source.pro -r -spec %QT_SPEC% "CONFIG+=release"

nmake Makefile.Release clean
nmake
del Makefile
del Makefile.Release
del Makefile.Debug

cd %SOURCE_PATH%

cd %SOURCE_PATH%\die_source\lite_source
%QT_PATH%\bin\qmake.exe lite_source.pro -r -spec %QT_SPEC% "CONFIG+=release"

nmake Makefile.Release clean
nmake
del Makefile
del Makefile.Release
del Makefile.Debug

cd %SOURCE_PATH%

cd %SOURCE_PATH%\die_source\diedll
%QT_PATH%\bin\qmake.exe diedll.pro -r -spec %QT_SPEC% "CONFIG+=release"

nmake Makefile.Release clean
nmake
del Makefile
del Makefile.Release
del Makefile.Debug

cd %SOURCE_PATH%

cd %SOURCE_PATH%\die_source\winloader_source

Rc.exe /v rsrc_die.rc
Cvtres.exe /v rsrc_die.res
cl.exe /c winloader_die.cpp
link.exe /SUBSYSTEM:WINDOWS winloader_die.obj rsrc_die.res user32.lib kernel32.lib shell32.lib /SUBSYSTEM:WINDOWS /ENTRY:entry /OUT:%SOURCE_PATH%\die_source\build\release\winloader_die.exe
if exist winloader_die.obj del winloader_die.obj
if exist rsrc_die.res del rsrc_die.res
if exist rsrc_die.obj del rsrc_die.obj

Rc.exe /v rsrc_diel.rc
Cvtres.exe /v rsrc_diel.res
cl.exe /c winloader_diel.cpp
link.exe /SUBSYSTEM:WINDOWS winloader_diel.obj rsrc_diel.res user32.lib kernel32.lib shell32.lib /SUBSYSTEM:WINDOWS /ENTRY:entry /OUT:%SOURCE_PATH%\die_source\build\release\winloader_diel.exe
if exist winloader_diel.obj del winloader_diel.obj
if exist rsrc_diel.res del rsrc_diel.res
if exist rsrc_diel.obj del rsrc_diel.obj

cd %SOURCE_PATH%

rem rmdir /s /q release\
mkdir %SOURCE_PATH%\release

rem portable
rmdir /s /q %SOURCE_PATH%\release\%BUILD_NAME%\
mkdir %SOURCE_PATH%\release\%BUILD_NAME%
mkdir %SOURCE_PATH%\release\%BUILD_NAME%\base
mkdir %SOURCE_PATH%\release\%BUILD_NAME%\base\platforms

copy %SOURCE_PATH%\die_source\build\release\winloader_die.exe %SOURCE_PATH%\release\%BUILD_NAME%\die.exe
copy %SOURCE_PATH%\die_source\build\release\winloader_diel.exe %SOURCE_PATH%\release\%BUILD_NAME%\diel.exe
copy %SOURCE_PATH%\die_source\build\release\die.exe %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %SOURCE_PATH%\die_source\build\release\diec.exe %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %SOURCE_PATH%\die_source\build\release\diel.exe %SOURCE_PATH%\release\%BUILD_NAME%\base\

copy %SOURCE_PATH%\die_source\files\win32\die.intermediate.manifest %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %SOURCE_PATH%\die_source\files\win32\diec.intermediate.manifest %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %SOURCE_PATH%\die_source\files\win32\diel.intermediate.manifest %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %SOURCE_PATH%\QOpenSSL\libs\win32-msvc\libcrypto-1_1.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\

copy %QT_PATH%\bin\Qt5Widgets.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %QT_PATH%\bin\Qt5Gui.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %QT_PATH%\bin\Qt5Core.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %QT_PATH%\bin\Qt5Script.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %QT_PATH%\bin\Qt5Network.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %QT_PATH%\bin\Qt5Concurrent.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %QT_PATH%\bin\Qt5ScriptTools.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %QT_PATH%\bin\Qt5Svg.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %QT_PATH%\bin\Qt5Xml.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %QT_PATH%\plugins\platforms\qwindows.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\platforms\

copy %VS_PATH%\VC\redist\x86\Microsoft.VC120.CRT\msvcp120.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %VS_PATH%\VC\redist\x86\Microsoft.VC120.CRT\msvcr120.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\

rem mkdir release\%BUILD_NAME%\base\die_plugins
rem copy plugins_source\build\release\simpleexample_win.dll release\%BUILD_NAME%\base\die_plugins\simpleexample.dll

xcopy %SOURCE_PATH%\Detect-It-Easy\db %SOURCE_PATH%\release\%BUILD_NAME%\base\db /E /I
xcopy %SOURCE_PATH%\Detect-It-Easy\editor %SOURCE_PATH%\release\%BUILD_NAME%\base\editor /E /I
xcopy %SOURCE_PATH%\Detect-It-Easy\qss %SOURCE_PATH%\release\%BUILD_NAME%\base\qss /E /I
xcopy %SOURCE_PATH%\Detect-It-Easy\info %SOURCE_PATH%\release\%BUILD_NAME%\base\info /E /I
xcopy %SOURCE_PATH%\Detect-It-Easy\scripts %SOURCE_PATH%\release\%BUILD_NAME%\base\scripts /E /I
xcopy %SOURCE_PATH%\Detect-It-Easy\search %SOURCE_PATH%\release\%BUILD_NAME%\base\search /E /I
xcopy %SOURCE_PATH%\die_source\lang %SOURCE_PATH%\release\%BUILD_NAME%\base\lang /E /I
xcopy %SOURCE_PATH%\Detect-It-Easy\yara %SOURCE_PATH%\release\%BUILD_NAME%\base\yara /E /I

%UPX_PATH%\upx.exe %SOURCE_PATH%\release\%BUILD_NAME%\base\die.exe
%UPX_PATH%\upx.exe %SOURCE_PATH%\release\%BUILD_NAME%\base\diel.exe
%UPX_PATH%\upx.exe %SOURCE_PATH%\release\%BUILD_NAME%\base\diec.exe

%UPX_PATH%\upx.exe %SOURCE_PATH%\release\%BUILD_NAME%\base\Qt5Widgets.dll
%UPX_PATH%\upx.exe %SOURCE_PATH%\release\%BUILD_NAME%\base\Qt5Gui.dll
%UPX_PATH%\upx.exe %SOURCE_PATH%\release\%BUILD_NAME%\base\Qt5Core.dll
%UPX_PATH%\upx.exe %SOURCE_PATH%\release\%BUILD_NAME%\base\Qt5Concurrent.dll
%UPX_PATH%\upx.exe %SOURCE_PATH%\release\%BUILD_NAME%\base\Qt5Script.dll
%UPX_PATH%\upx.exe %SOURCE_PATH%\release\%BUILD_NAME%\base\Qt5Network.dll
%UPX_PATH%\upx.exe %SOURCE_PATH%\release\%BUILD_NAME%\base\Qt5ScriptTools.dll
%UPX_PATH%\upx.exe %SOURCE_PATH%\release\%BUILD_NAME%\base\Qt5Svg.dll
%UPX_PATH%\upx.exe %SOURCE_PATH%\release\%BUILD_NAME%\base\Qt5Xml.dll
%UPX_PATH%\upx.exe %SOURCE_PATH%\release\%BUILD_NAME%\base\libcrypto-1_1.dll

rmdir /s /q %SOURCE_PATH%\release\%BUILD_NAME_DLL%\
mkdir %SOURCE_PATH%\release\%BUILD_NAME_DLL%
copy %SOURCE_PATH%\die_source\build\release\diedll.dll release\%BUILD_NAME_DLL%\
copy %SOURCE_PATH%\die_source\build\release\diedll.lib release\%BUILD_NAME_DLL%\
copy %QT_PATH%\bin\Qt5Core.dll %SOURCE_PATH%\release\%BUILD_NAME_DLL%\
copy %QT_PATH%\bin\Qt5Script.dll %SOURCE_PATH%\release\%BUILD_NAME_DLL%\

copy %VS_PATH%\VC\redist\x86\Microsoft.VC120.CRT\msvcp120.dll %SOURCE_PATH%\release\%BUILD_NAME_DLL%\
copy %VS_PATH%\VC\redist\x86\Microsoft.VC120.CRT\msvcr120.dll %SOURCE_PATH%\release\%BUILD_NAME_DLL%\

xcopy %SOURCE_PATH%\db %SOURCE_PATH%\release\%BUILD_NAME_DLL%\db /E /I
xcopy %SOURCE_PATH%\diedll_win\SDK %SOURCE_PATH%\release\%BUILD_NAME_DLL%\SDK /E /I
copy %SOURCE_PATH%\diedll_win\diedll.h %SOURCE_PATH%\release\%BUILD_NAME_DLL%\

cd %SOURCE_PATH%\release
if exist %BUILD_NAME%_%RELEASE_VERSION%.zip del %BUILD_NAME%_%RELEASE_VERSION%.zip
%SEVENZIP_PATH%\7z.exe a %BUILD_NAME%_%RELEASE_VERSION%.zip %BUILD_NAME%\*
rmdir /s /q %BUILD_NAME%\
if exist %BUILD_NAME_DLL%_%RELEASE_VERSION%.zip del %BUILD_NAME_DLL%_%RELEASE_VERSION%.zip
%SEVENZIP_PATH%\7z.exe a %BUILD_NAME_DLL%_%RELEASE_VERSION%.zip %BUILD_NAME_DLL%\*
rmdir /s /q %BUILD_NAME_DLL%\
cd %SOURCE_PATH%

goto:eof
