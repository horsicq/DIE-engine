call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\bin\vcvars32.bat"

set JOM="C:\Qt\jom\jom.exe"
set QMAKE="C:\Qt\4.8.6\bin\qmake.exe"

%QMAKE% DIE_source/dielib/dielib.pro -spec win32-msvc2010
%JOM% -j 8 -f Makefile.Release clean
%JOM% -j 8 -f Makefile.Release

del Makefile
del Makefile.Release
del Makefile.Debug
