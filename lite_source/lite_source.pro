QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
TARGET = diel

macx {
    TARGET = DiEL
}

include(../build.pri)

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

DISTFILES += \
    CMakeLists.txt
