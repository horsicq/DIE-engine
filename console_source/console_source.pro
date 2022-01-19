QT += core
QT -= gui

XCONFIG += use_dex
XCONFIG += use_archive

CONFIG += c++11

TARGET = diec
CONFIG += console
CONFIG -= app_bundle

include(../build.pri)

XCONFIG += use_capstone_x86

SOURCES += \
        consoleoutput.cpp \
        main_console.cpp

HEADERS += \
    consoleoutput.h

!contains(XCONFIG, die_script) {
    XCONFIG += die_script
    include(../die_script/die_script.pri)
}

!contains(XCONFIG, entropyprocess) {
    XCONFIG += entropyprocess
    include(../XEntropyWidget/entropyprocess.pri)
}

!contains(XCONFIG, xfileinfo) {
    XCONFIG += xfileinfo
    include(../XFileInfo/xfileinfo.pri)
}

!contains(XCONFIG, xoptions) {
    XCONFIG += xoptions
    include(../XOptions/xoptions.pri)
}

win32 {
    CONFIG -= embed_manifest_exe
    QMAKE_MANIFEST = windows.manifest.xml
}

DISTFILES += \
    CMakeLists.txt
