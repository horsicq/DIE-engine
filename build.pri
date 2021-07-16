# For additional build parameters
CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/build/debug
} else {
    DESTDIR = $$PWD/build/release
}

win32-g++ {
    QMAKE_CXXFLAGS += -Wno-missing-field-initializers
    QMAKE_CXXFLAGS += -Werror=format-security
}
unix:!macx {
    QMAKE_CXXFLAGS += -Wno-missing-field-initializers
    QMAKE_CXXFLAGS += -Werror=format-security
}
unix:macx {
    QMAKE_CXXFLAGS += -Wno-missing-field-initializers
    QMAKE_CXXFLAGS += -Werror=format-security
}

!contains(QMAKE_TARGET.arch, x86_64) {
    win32-msvc2013{
       # VS2013
       # Windows XP support
       QMAKE_CXXFLAGS  += /D_USING_V110_SDK71_
       QMAKE_LFLAGS += /SUBSYSTEM:WINDOWS,5.01
    }
    win32-msvc{ 
       # VS2017
       # Windows XP support
       QMAKE_CXXFLAGS  += /D_USING_V110_SDK71_
       QMAKE_LFLAGS += /SUBSYSTEM:WINDOWS,5.01
    }
    # TODO more MSVC versions
}
