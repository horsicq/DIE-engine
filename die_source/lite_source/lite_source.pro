QT       += core gui script xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = diel

include(../../build.pri)

win32 {
    RC_ICONS = lite.ico
}

HEADERS += \
    ../../pefile.h \
    ../../msdosfile.h \
    ../../elffile.h \
    ../../textfile.h \
    ../../binary.h \
    ../scriptdie.h \
    ../scriptpe.h \
    ../scriptmsdos.h \
    ../scriptelf.h \
    ../scriptbinary.h \
    ../scripttext.h \
    ../scan.h \
    ../utils.h \
    ../../pluginsscript.h \
    ../_options.h \
    ../../versionresource.h \
    ../../machfile.h \
    ../scriptmach.h \
    ../scriptgeneric.h \
    litewindow.h

SOURCES += \
    ../../pefile.cpp \
    ../../msdosfile.cpp \
    ../../elffile.cpp \
    ../../textfile.cpp \
    ../../binary.cpp \
    ../scriptdie.cpp \
    ../scriptpe.cpp \
    ../scriptmsdos.cpp \
    ../scriptelf.cpp \
    ../scriptbinary.cpp \
    ../scripttext.cpp \
    ../scan.cpp \
    ../utils.cpp \
    ../../pluginsscript.cpp \
    ../../versionresource.cpp \
    ../../machfile.cpp \
    ../scriptmach.cpp \
    ../scriptgeneric.cpp \
    litewindow.cpp \
    lite.cpp

FORMS += \
    litewindow.ui
