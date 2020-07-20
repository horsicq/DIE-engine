QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    dialogabout.cpp \
    dialogoptions.cpp \
    guimainwindow.cpp \
    main_gui.cpp

HEADERS += \
    dialogabout.h \
    dialogoptions.h \
    guimainwindow.h

FORMS += \
    dialogabout.ui \
    dialogoptions.ui \
    guimainwindow.ui

include(../build.pri)

!contains(XCONFIG, formatwidgets) {
    XCONFIG += formatwidgets
    include(../../_mylibs/FormatWidgets/formatwidgets.pri)
}

!contains(XCONFIG, die_widget) {
    XCONFIG += die_widget
    include(../../_mylibs/die_widget/die_widget.pri)
}

!contains(XCONFIG, xoptions) {
    XCONFIG += xoptions
    include(../../_mylibs/XOptions/xoptions.pri)
}
