set VS_PATH="C:\Program Files (x86)\Microsoft Visual Studio 12.0"
set SEVENZIP_PATH="C:\Program Files\7-Zip"
set QT_PATH=C:\Qt5.6.3\5.6.3\msvc2013

set BUILD_NAME=die_winxp_portable
set SOURCE_PATH=%~dp0
mkdir %SOURCE_PATH%\build
mkdir %SOURCE_PATH%\build\loader
mkdir %SOURCE_PATH%\release
set /p RELEASE_VERSION=<%SOURCE_PATH%\release_version.txt

set QT_PATH=%QT_PATH%
set QT_SPEC=win32-msvc2013
call %VS_PATH%\VC\bin\vcvars32.bat
set GUIEXE=die.exe
set CONEXE=diec.exe
set ZIP_NAME=%BUILD_NAME%_%RELEASE_VERSION%
set RES_FILE=rsrc

cd build_libs
%QT_PATH%\bin\qmake.exe build_libs.pro -r -spec %QT_SPEC% "CONFIG+=release"

nmake Makefile.Release clean
nmake
del Makefile
del Makefile.Release
del Makefile.Debug

cd ..

cd gui_source
%QT_PATH%\bin\qmake.exe gui_source.pro -r -spec %QT_SPEC% "CONFIG+=release"
%QT_PATH%\bin\lupdate.exe gui_source_tr.pro

nmake Makefile.Release clean
nmake
del Makefile
del Makefile.Release
del Makefile.Debug

cd ..

cd console_source
%QT_PATH%\bin\qmake.exe console_source.pro -r -spec %QT_SPEC% "CONFIG+=release"

nmake Makefile.Release clean
nmake
del Makefile
del Makefile.Release
del Makefile.Debug

cd ..

cd winloader_source

cl.exe /c winloader.cpp /D_USING_V110_SDK71_ /GS- /Oi-
Rc.exe /v %RES_FILE%.rc
link.exe winloader.obj %RES_FILE%.res user32.lib kernel32.lib shell32.lib /NODEFAULTLIB /SAFESEH:NO /SUBSYSTEM:WINDOWS,5.01 /ENTRY:entry /OUT:%SOURCE_PATH%\build\loader\%GUIEXE%
del /s winloader.obj
del /s %RES_FILE%.res

cd ..

mkdir %SOURCE_PATH%\release\%BUILD_NAME%
mkdir %SOURCE_PATH%\release\%BUILD_NAME%\base
mkdir %SOURCE_PATH%\release\%BUILD_NAME%\base\lang
mkdir %SOURCE_PATH%\release\%BUILD_NAME%\base\platforms

copy %SOURCE_PATH%\build\loader\%GUIEXE% %SOURCE_PATH%\release\%BUILD_NAME%\
copy %SOURCE_PATH%\build\release\%GUIEXE% %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %SOURCE_PATH%\build\release\%CONEXE% %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %QT_PATH%\bin\Qt5Widgets.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %QT_PATH%\bin\Qt5Gui.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %QT_PATH%\bin\Qt5Core.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %QT_PATH%\bin\Qt5OpenGL.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %QT_PATH%\bin\Qt5Svg.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %QT_PATH%\bin\Qt5Script.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %QT_PATH%\bin\Qt5ScriptTools.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %QT_PATH%\plugins\platforms\qwindows.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\platforms\

copy %VS_PATH%\VC\redist\x86\Microsoft.VC120.CRT\msvcp120.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %VS_PATH%\VC\redist\x86\Microsoft.VC120.CRT\msvcr120.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\

xcopy %SOURCE_PATH%\Detect-It-Easy\db %SOURCE_PATH%\release\%BUILD_NAME%\base\db /E /I
xcopy %SOURCE_PATH%\Detect-It-Easy\info %SOURCE_PATH%\release\%BUILD_NAME%\base\info /E /I
xcopy %SOURCE_PATH%\XStyles\qss %SOURCE_PATH%\release\%BUILD_NAME%\base\qss /E /I

%QT_PATH%\bin\lrelease.exe %SOURCE_PATH%\gui_source\translation\die_de.ts -qm %SOURCE_PATH%\release\%BUILD_NAME%\base\lang\die_de.qm
rem %QT_PATH%\bin\lrelease.exe %SOURCE_PATH%\gui_source\translation\die_ar.ts -qm %SOURCE_PATH%\release\%BUILD_NAME%\base\lang\die_ar.qm
rem %QT_PATH%\bin\lrelease.exe %SOURCE_PATH%\gui_source\translation\die_es.ts -qm %SOURCE_PATH%\release\%BUILD_NAME%\base\lang\die_es.qm
%QT_PATH%\bin\lrelease.exe %SOURCE_PATH%\gui_source\translation\die_ja.ts -qm %SOURCE_PATH%\release\%BUILD_NAME%\base\lang\die_ja.qm
%QT_PATH%\bin\lrelease.exe %SOURCE_PATH%\gui_source\translation\die_pl.ts -qm %SOURCE_PATH%\release\%BUILD_NAME%\base\lang\die_pl.qm
%QT_PATH%\bin\lrelease.exe %SOURCE_PATH%\gui_source\translation\die_pt_BR.ts -qm %SOURCE_PATH%\release\%BUILD_NAME%\base\lang\die_pt_BR.qm
%QT_PATH%\bin\lrelease.exe %SOURCE_PATH%\gui_source\translation\die_fr.ts -qm %SOURCE_PATH%\release\%BUILD_NAME%\base\lang\die_fr.qm
%QT_PATH%\bin\lrelease.exe %SOURCE_PATH%\gui_source\translation\die_ru.ts -qm %SOURCE_PATH%\release\%BUILD_NAME%\base\lang\die_ru.qm
%QT_PATH%\bin\lrelease.exe %SOURCE_PATH%\gui_source\translation\die_vi.ts -qm %SOURCE_PATH%\release\%BUILD_NAME%\base\lang\die_vi.qm
%QT_PATH%\bin\lrelease.exe %SOURCE_PATH%\gui_source\translation\die_zh.ts -qm %SOURCE_PATH%\release\%BUILD_NAME%\base\lang\die_zh.qm
%QT_PATH%\bin\lrelease.exe %SOURCE_PATH%\gui_source\translation\die_zh_TW.ts -qm %SOURCE_PATH%\release\%BUILD_NAME%\base\lang\die_zh_TW.qm
%QT_PATH%\bin\lrelease.exe %SOURCE_PATH%\gui_source\translation\die_it.ts -qm %SOURCE_PATH%\release\%BUILD_NAME%\base\lang\die_it.qm
%QT_PATH%\bin\lrelease.exe %SOURCE_PATH%\gui_source\translation\die_ko.ts -qm %SOURCE_PATH%\release\%BUILD_NAME%\base\lang\die_ko.qm

cd %SOURCE_PATH%\release
if exist %ZIP_NAME%.zip del %ZIP_NAME%.zip
%SEVENZIP_PATH%\7z.exe a %ZIP_NAME%.zip %BUILD_NAME%\*
rmdir /s /q %SOURCE_PATH%\release\%BUILD_NAME%
cd ..