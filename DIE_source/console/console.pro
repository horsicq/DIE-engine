QT       += core script xml concurrent

QT       -= gui

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x000000

CONFIG   += console
CONFIG   -= app_bundle

TARGET = DIEC
win32 {
TARGET = DIEC_win
}
unix:!macx {
BITSIZE = $$system(getconf LONG_BIT)
if (contains(BITSIZE, 64)) {
  TARGET = DIEC_lin64
}
if (contains(BITSIZE, 32)) {
  TARGET = DIEC_lin32
}
}
unix:macx {
TARGET = DIEC_mac
}

include(../../build.pri)

HEADERS += \
    _mainclass.h \
    ../scriptpe.h \
    ../scriptmsdos.h \
    ../scriptelf.h \
    ../scriptbinary.h \
    ../scan.h \
    ../utils.h \
    ../../pluginsscript.h \
    ../_options.h \
    ../../versionresource.h \
    ../../pefile.h \
    ../../msdosfile.h \
    ../../elffile.h \
    ../../textfile.h \
    ../../binary.h \
    ../../machfile.h \
    ../scriptmach.h \
    ../scripttext.h \
    ../scriptgeneric.h \
    ../singlefilescan.h

SOURCES += \
    mainc.cpp \
    _mainclass.cpp \
    ../scriptpe.cpp \
    ../scriptmsdos.cpp \
    ../scriptelf.cpp \
    ../scriptbinary.cpp \
    ../scan.cpp \
    ../utils.cpp \
    ../../pluginsscript.cpp \
    ../../versionresource.cpp \
    ../../pefile.cpp \
    ../../msdosfile.cpp \
    ../../elffile.cpp \
    ../../textfile.cpp \
    ../../binary.cpp \
    ../../machfile.cpp \
    ../scriptmach.cpp \
    ../scripttext.cpp \
    ../scriptgeneric.cpp \
    ../singlefilescan.cpp

