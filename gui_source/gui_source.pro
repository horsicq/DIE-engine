QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
TARGET = die

macx {
    TARGET = DiE
}

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

!contains(XCONFIG, xmimewidget) {
    XCONFIG += xmimewidget
    include(../XMIMEWidget/xmimewidget.pri)
}

!contains(XCONFIG, formatwidgets) {
    XCONFIG += formatwidgets
    include(../FormatWidgets/formatwidgets.pri)
}

!contains(XCONFIG, xsingleapplication) {
    XCONFIG += xsingleapplication
    include(../XSingleApplication/xsingleapplication.pri)
}

RESOURCES += \
    res.qrc

win32 {
    RC_ICONS = ../icons/main.ico
}

macx {
    ICON = ../icons/main.icns
}

DISTFILES += \
    CMakeLists.txt
