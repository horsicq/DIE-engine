INCLUDEPATH += $$PWD/headers
DEPENDPATH += $$PWD/headers

win32 {
    QMAKE_LFLAGS_DEBUG += /NODEFAULTLIB:crt.lib
    QMAKE_LFLAGS_RELEASE += /NODEFAULTLIB:crt.lib

    LIBS += $$PWD/libs/win32/BeaEngine_s_d_l.lib
}
unix:!macx {
BITSIZE = $$system(getconf LONG_BIT)
if (contains(BITSIZE, 64)) {
LIBS +=  $$PWD/libs/lin64/libBeaEngine.a
}
if (contains(BITSIZE, 32)) {
# TODO
}
}

unix:macx {
LIBS +=  $$PWD/libs/mac/libBeaEngine.a
}

