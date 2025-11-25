QT += core
QT -= gui

XCONFIG += use_dex
XCONFIG += use_archive
XCONFIG += use_pdf

CONFIG += c++11

TARGET = diec
CONFIG += console
CONFIG -= app_bundle

DEFINES  += NOMINMAX
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
    VERSION = 3.10.0.0
    QMAKE_TARGET_COMPANY = NTInfo
    QMAKE_TARGET_PRODUCT = Detect It Easy
    QMAKE_TARGET_DESCRIPTION = Detect It Easy(DiE)
    QMAKE_TARGET_COPYRIGHT = horsicq@gmail.com
}

DISTFILES += \
    CMakeLists.txt
