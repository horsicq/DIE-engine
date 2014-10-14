#-------------------------------------------------
#
# Project created by QtCreator 2014-02-05T13:18:36
#
#-------------------------------------------------

QT       +=script xml gui

TARGET = dielib

win32 {
    TARGET = dielib_win32
}
unix:!macx {

    BITSIZE = $$system(getconf LONG_BIT)
    if (contains(BITSIZE, 64)) {
        TARGET = dielib_lin64
    }
    if (contains(BITSIZE, 32)) {
        TARGET = dielib_lin32
    }
}

unix:macx {
    TARGET = dielib_mac
}

TEMPLATE = lib
CONFIG+= staticlib

SOURCES += dielib.cpp \
    ../scriptpe.cpp \
    ../scriptmsdos.cpp \
    ../scriptelf.cpp \
    ../scriptbinary.cpp \
    ../scripttext.cpp \
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
    ../scriptgeneric.cpp \
    ../singlefilescan.cpp

HEADERS += dielib.h \
    ../scriptpe.h \
    ../scriptmsdos.h \
    ../scriptelf.h \
    ../scriptbinary.h \
    ../scripttext.h \
    ../scan.h \
    ../utils.h \
    ../../pluginsscript.h \
    ../../versionresource.h \
    ../../pefile.h \
    ../../msdosfile.h \
    ../../elffile.h \
    ../../textfile.h \
    ../../binary.h \
    ../../machfile.h \
    ../scriptmach.h \
    ../scriptgeneric.h \
    ../singlefilescan.h

