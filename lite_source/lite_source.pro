QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
TARGET = diel

XCONFIG += use_dex
XCONFIG += use_pdf
XCONFIG += use_archive
XCONFIG += use_die
XCONFIG += use_xsimd

macx {
    TARGET = DiEL
}

DEFINES += NOMINMAX
include(../build.pri)

XCONFIG += use_capstone_x86

SOURCES += \
    litemainwindow.cpp \
    main_lite.cpp

HEADERS += \
    litemainwindow.h

FORMS += \
    litemainwindow.ui

!contains(XCONFIG, die_script) {
    XCONFIG += die_script
    include(../die_script/die_script.pri)
}

!contains(XCONFIG, xoptions) {
    XCONFIG += xoptions
    include(../XOptions/xoptions.pri)
}

!contains(XCONFIG, xcomboboxex) {
    XCONFIG += xcomboboxex
    include(../Controls/xcomboboxex.pri)
}

win32 {
    RC_ICONS = ../icons/lite.ico
    CONFIG -= embed_manifest_exe
    QMAKE_MANIFEST = $$PWD/windows.manifest.xml
    VERSION = 3.11
    QMAKE_TARGET_COMPANY = NTInfo
    QMAKE_TARGET_PRODUCT = Detect It Easy
    QMAKE_TARGET_DESCRIPTION = Detect It Easy (DiE)
    QMAKE_TARGET_COPYRIGHT = Copyright (C) 2012-2026 Hors
}

macx {
    ICON = ../icons/lite.icns
}

DISTFILES += \
    CMakeLists.txt
