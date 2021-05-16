set VS_PATH="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community"
set QT_PATH=C:\Qt\5.15.2\msvc2019
set SEVENZIP_PATH="C:\Program Files\7-Zip"

set BUILD_NAME=die_win32_portable
set SOURCE_PATH=%~dp0
mkdir %SOURCE_PATH%\build
mkdir %SOURCE_PATH%\release
set /p RELEASE_VERSION=<%SOURCE_PATH%\release_version.txt

set QT_PATH=%QT_PATH%
call %VS_PATH%\VC\Auxiliary\Build\vcvars32.bat
set GUIEXE=die.exe
set CONEXE=diec.exe
set ZIP_NAME=%BUILD_NAME%_%RELEASE_VERSION%
set RES_FILE=rsrc

del %SOURCE_PATH%\XArchive\.qmake.stash
del %SOURCE_PATH%\XCapstone\.qmake.stash
del %SOURCE_PATH%\build_libs\.qmake.stash
del %SOURCE_PATH%\gui_source\.qmake.stash
del %SOURCE_PATH%\console_source\.qmake.stash

rmdir /s /q %SOURCE_PATH%\XCapstone\3rdparty\Capstone\release
rmdir /s /q %SOURCE_PATH%\XArchive\3rdparty\lzma\release
rmdir /s /q %SOURCE_PATH%\XArchive\3rdparty\zlib\release
rmdir /s /q %SOURCE_PATH%\XArchive\3rdparty\bzip2\release

rmdir /s /q %SOURCE_PATH%\console_source\release
rmdir /s /q %SOURCE_PATH%\gui_source\release

cd build_libs
%QT_PATH%\bin\qmake.exe build_libs.pro -r -spec win32-msvc "CONFIG+=release"

nmake Makefile clean
nmake
del Makefile
del Makefile.Release
del Makefile.Debug

cd ..

cd gui_source
%QT_PATH%\bin\qmake.exe gui_source.pro -r -spec win32-msvc "CONFIG+=release"
%QT_PATH%\bin\lupdate.exe gui_source_tr.pro
%QT_PATH%\bin\lrelease.exe gui_source_tr.pro

nmake Makefile.Release clean
nmake
del Makefile
del Makefile.Release
del Makefile.Debug

cd ..

cd console_source
%QT_PATH%\bin\qmake.exe console_source.pro -r -spec win32-msvc "CONFIG+=release"

nmake Makefile.Release clean
nmake
del Makefile
del Makefile.Release
del Makefile.Debug

cd ..

mkdir %SOURCE_PATH%\release\%BUILD_NAME%
mkdir %SOURCE_PATH%\release\%BUILD_NAME%\platforms
mkdir %SOURCE_PATH%\release\%BUILD_NAME%\imageformats
mkdir %SOURCE_PATH%\release\%BUILD_NAME%\lang

copy %SOURCE_PATH%\build\release\%GUIEXE% %SOURCE_PATH%\release\%BUILD_NAME%\
copy %SOURCE_PATH%\build\release\%CONEXE% %SOURCE_PATH%\release\%BUILD_NAME%\
copy %QT_PATH%\bin\Qt5Widgets.dll %SOURCE_PATH%\release\%BUILD_NAME%\
copy %QT_PATH%\bin\Qt5Gui.dll %SOURCE_PATH%\release\%BUILD_NAME%\
copy %QT_PATH%\bin\Qt5Core.dll %SOURCE_PATH%\release\%BUILD_NAME%\
copy %QT_PATH%\bin\Qt5Svg.dll %SOURCE_PATH%\release\%BUILD_NAME%\
copy %QT_PATH%\bin\Qt5OpenGL.dll %SOURCE_PATH%\release\%BUILD_NAME%\
copy %QT_PATH%\bin\Qt5Script.dll %SOURCE_PATH%\release\%BUILD_NAME%\
copy %QT_PATH%\bin\Qt5ScriptTools.dll %SOURCE_PATH%\release\%BUILD_NAME%\
copy %QT_PATH%\bin\Qt5Network.dll %SOURCE_PATH%\release\%BUILD_NAME%\
copy %QT_PATH%\plugins\platforms\qwindows.dll %SOURCE_PATH%\release\%BUILD_NAME%\platforms\
copy %QT_PATH%\plugins\imageformats\qjpeg.dll %SOURCE_PATH%\release\%BUILD_NAME%\imageformats\
copy %QT_PATH%\plugins\imageformats\qtiff.dll %SOURCE_PATH%\release\%BUILD_NAME%\imageformats\
copy %QT_PATH%\plugins\imageformats\qico.dll %SOURCE_PATH%\release\%BUILD_NAME%\imageformats\
copy %QT_PATH%\plugins\imageformats\qgif.dll %SOURCE_PATH%\release\%BUILD_NAME%\imageformats\

copy %VS_PATH%\VC\Redist\MSVC\%VCToolsVersion%\x86\Microsoft.VC142.CRT\msvcp140.dll %SOURCE_PATH%\release\%BUILD_NAME%\
copy %VS_PATH%\VC\Redist\MSVC\%VCToolsVersion%\x86\Microsoft.VC142.CRT\vcruntime140.dll %SOURCE_PATH%\release\%BUILD_NAME%\
copy %VS_PATH%\VC\Redist\MSVC\%VCToolsVersion%\x86\Microsoft.VC142.CRT\msvcp140_1.dll %SOURCE_PATH%\release\%BUILD_NAME%\

xcopy %SOURCE_PATH%\XStyles\qss %SOURCE_PATH%\release\%BUILD_NAME%\qss /E /I
xcopy %SOURCE_PATH%\Detect-It-Easy\db %SOURCE_PATH%\release\%BUILD_NAME%\db /E /I
xcopy %SOURCE_PATH%\Detect-It-Easy\info %SOURCE_PATH%\release\%BUILD_NAME%\info /E /I

move %SOURCE_PATH%\gui_source\translation\*.qm  %SOURCE_PATH%\release\%BUILD_NAME%\lang\

mkdir %SOURCE_PATH%\release\%BUILD_NAME%\signatures
xcopy %SOURCE_PATH%\signatures\crypto.db %SOURCE_PATH%\release\%BUILD_NAME%\signatures\

cd %SOURCE_PATH%\release
if exist %ZIP_NAME%.zip del %ZIP_NAME%.zip
%SEVENZIP_PATH%\7z.exe a %ZIP_NAME%.zip %BUILD_NAME%\*
rmdir /s /q %SOURCE_PATH%\release\%BUILD_NAME%
cd ..
