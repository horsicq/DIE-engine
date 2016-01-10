#-------------------------------------------------
#
# Project created by QtCreator 2014-02-05T13:18:36
#
#-------------------------------------------------

QT       +=script xml gui

TARGET = diedll
TEMPLATE = lib

include(../../build.pri)

DEFINES += DIEDLL_LIBRARY

SOURCES += diedll.cpp \
    ../scriptpe.cpp \
    ../scriptmsdos.cpp \
    ../scriptelf.cpp \
    ../scriptbinary.cpp \
    ../scripttext.cpp \
    ../scan.cpp \
    ../utils.cpp \
    ../../versionresource.cpp \
    ../../pefile.cpp \
    ../../msdosfile.cpp \
    ../../elffile.cpp \
    ../../textfile.cpp \
    ../../binary.cpp \
    ../../pluginsscript.cpp \
    ../../machfile.cpp \
    ../scriptmach.cpp \
    ../scriptgeneric.cpp \
    ../singlefilescan.cpp

HEADERS += diedll.h \
    ../scriptpe.h \
    ../scriptmsdos.h \
    ../scriptelf.h \
    ../scriptbinary.h \
    ../scripttext.h \
    ../scan.h \
    ../utils.h \
    ../../versionresource.h \
    ../../pefile.h \
    ../../msdosfile.h \
    ../../elffile.h \
    ../../textfile.h \
    ../../binary.h \
    ../../pluginsscript.h \
    ../../machfile.h \
    ../scriptmach.h \
    ../scriptgeneric.h \
    ../singlefilescan.h


unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
