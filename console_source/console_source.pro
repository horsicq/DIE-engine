QT += core
QT -= gui

include(../build.pri)

XCONFIG += use_dex

CONFIG += c++11

TARGET = diec
CONFIG += console
CONFIG -= app_bundle

SOURCES += \
        main_console.cpp

!contains(XCONFIG, die_script) {
    XCONFIG += die_script
    include(../../_mylibs/die_script/die_script.pri)
}
