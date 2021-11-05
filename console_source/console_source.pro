QT += core
QT -= gui

XCONFIG += use_dex

CONFIG += c++11

TARGET = diec
CONFIG += console
CONFIG -= app_bundle

include(../build.pri)

SOURCES += \
        main_console.cpp

!contains(XCONFIG, die_script) {
    XCONFIG += die_script
    include(../die_script/die_script.pri)
}

!contains(XCONFIG, entropyprocess) {
    XCONFIG += entropyprocess
    include(../XEntropyWidget/entropyprocess.pri)
}

!contains(XCONFIG, xoptions) {
    XCONFIG += xoptions
    include(../XOptions/xoptions.pri)
}

DISTFILES += \
    CMakeLists.txt
