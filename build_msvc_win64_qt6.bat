set VS_VERSIONS=2022 2019 2017
set VS_EDITIONS=Community Professional Enterprise
set QT_BASE_PATH="C:\Qt"

set VSVARS_PATH=
for %%V in (%VS_VERSIONS%) do (
    for %%E in (%VS_EDITIONS%) do (
        IF EXIST "C:\Program Files\Microsoft Visual Studio\%%V\%%E\VC\Auxiliary\Build\vcvars64.bat" (
            set VSVARS_PATH="C:\Program Files\Microsoft Visual Studio\%%V\%%E\VC\Auxiliary\Build\vcvars64.bat"
            goto :found_vs
        )
		
        if exist "C:\Program Files (x86)\Microsoft Visual Studio\%%V\%%E\VC\Auxiliary\Build\vcvars64.bat" (
            set "VSVARS_PATH=C:\Program Files (x86)\Microsoft Visual Studio\%%V\%%E\VC\Auxiliary\Build\vcvars64.bat"
            goto :found_vs
        )
    )
)

:found_vs
IF NOT DEFINED VSVARS_PATH (
    echo "Visual Studio 2022, 2019, or 2017 not found. Please ensure one of them is installed."
    goto exit
)

set QMAKE_PATH=
for /D %%Q in (%QT_BASE_PATH%\6.*) do (
    IF EXIST "%%Q\msvc2019_64\bin\qmake.exe" (
        set QMAKE_PATH="%%Q\msvc2019_64\bin\qmake.exe"
        goto :found_qt
    )
)

:found_qt
IF NOT DEFINED QMAKE_PATH (
    echo "Qt 6.x version not found. Please ensure it is installed."
    goto exit
)

set SEVENZIP_PATH="C:\Program Files\7-Zip\7z.exe"
set INNOSETUP_PATH="C:\Program Files (x86)\Inno Setup 6\ISCC.exe"

set X_SOURCE_PATH=%~dp0
set X_BUILD_NAME=die
set X_BUILD_PREFIX=win64_qt6
set /p X_RELEASE_VERSION=<%X_SOURCE_PATH%\release_version.txt

call %X_SOURCE_PATH%\build_tools\windows.cmd make_init

IF NOT [%X_ERROR%] == [] goto exit

call %X_SOURCE_PATH%\build_tools\windows.cmd make_build %X_SOURCE_PATH%\die_source.pro

cd %X_SOURCE_PATH%\gui_source
call %X_SOURCE_PATH%\build_tools\windows.cmd make_translate gui_source_tr.pro 
cd %X_SOURCE_PATH%

call %X_SOURCE_PATH%\build_tools\windows.cmd check_file %X_SOURCE_PATH%\build\release\die.exe

IF NOT [%X_ERROR%] == [] goto exit

call %X_SOURCE_PATH%\build_tools\windows.cmd check_file %X_SOURCE_PATH%\build\release\diec.exe

IF NOT [%X_ERROR%] == [] goto exit

call %X_SOURCE_PATH%\build_tools\windows.cmd check_file %X_SOURCE_PATH%\build\release\diel.exe

IF NOT [%X_ERROR%] == [] goto exit

mkdir %X_SOURCE_PATH%\release\%X_BUILD_NAME%\signatures

copy %X_SOURCE_PATH%\build\release\die.exe %X_SOURCE_PATH%\release\%X_BUILD_NAME%\
copy %X_SOURCE_PATH%\build\release\diec.exe %X_SOURCE_PATH%\release\%X_BUILD_NAME%\
copy %X_SOURCE_PATH%\build\release\diel.exe %X_SOURCE_PATH%\release\%X_BUILD_NAME%\
xcopy %X_SOURCE_PATH%\XStyles\qss %X_SOURCE_PATH%\release\%X_BUILD_NAME%\qss /E /I
xcopy %X_SOURCE_PATH%\Detect-It-Easy\db %X_SOURCE_PATH%\release\%X_BUILD_NAME%\db /E /I
xcopy %X_SOURCE_PATH%\Detect-It-Easy\info %X_SOURCE_PATH%\release\%X_BUILD_NAME%\info /E /I
xcopy %X_SOURCE_PATH%\signatures\crypto.db %X_SOURCE_PATH%\release\%X_BUILD_NAME%\signatures\
xcopy %X_SOURCE_PATH%\images %X_SOURCE_PATH%\release\%X_BUILD_NAME%\images /E /I
xcopy %X_SOURCE_PATH%\XYara\yara_rules %X_SOURCE_PATH%\release\%X_BUILD_NAME%\yara_rules /E /I

call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt die.exe

call %X_SOURCE_PATH%\build_tools\windows.cmd make_release

:exit
call %X_SOURCE_PATH%\build_tools\windows.cmd make_clear
