TARGET = $$qtLibraryTarget(simpleexample)

win32{
    TARGET = $$qtLibraryTarget(simpleexample_win)
}

unix:!macx {

BITSIZE = $$system(getconf LONG_BIT)
if (contains(BITSIZE, 64)) {
  TARGET = $$qtLibraryTarget(simpleexample_lin64)
}
if (contains(BITSIZE, 32)) {
  TARGET = $$qtLibraryTarget(simpleexample_lin32)
}

}

unix:macx {

TARGET = $$qtLibraryTarget(simpleexample_mac)

}


TEMPLATE = lib
CONFIG += plugin

include(../../../build.pri)

SOURCES += simpleexample.cpp
HEADERS += simpleexample.h \
    ../plugininterface.h
