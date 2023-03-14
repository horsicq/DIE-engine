# For additional build parameters

CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/build/debug
} else {
    DESTDIR = $$PWD/build/release
}

#win32-g++ {
#   QMAKE_CXXFLAGS += -Werror
#}
#win32-msvc*{
#   QMAKE_CXXFLAGS += /WX
#}

# Windows minGW
win32-g++* {
    QMAKE_CFLAGS  += -Wno-missing-field-initializers
    QMAKE_CFLAGS  += -Werror=format-security
    QMAKE_CFLAGS  += -Wno-unused-parameter
    QMAKE_CXXFLAGS += -Wno-missing-field-initializers
    QMAKE_CXXFLAGS += -Werror=format-security
    QMAKE_CXXFLAGS += -Wno-unused-parameter
}
# Linux gcc
unix:!macx {
    QMAKE_CFLAGS  += -Wno-missing-field-initializers
    QMAKE_CFLAGS  += -Werror=format-security
    QMAKE_CFLAGS  += -Wno-unused-parameter
    QMAKE_CXXFLAGS += -Wno-missing-field-initializers
    QMAKE_CXXFLAGS += -Werror=format-security
    QMAKE_CXXFLAGS += -Wno-unused-parameter
}
# macOS clang
unix:macx {
    QMAKE_CFLAGS  += -Wno-missing-field-initializers
    QMAKE_CFLAGS  += -Werror=format-security
    QMAKE_CFLAGS  += -Wno-unused-parameter
    QMAKE_CXXFLAGS += -Wno-missing-field-initializers
    QMAKE_CXXFLAGS += -Werror=format-security
    QMAKE_CXXFLAGS += -Wno-unused-parameter
}
# Windows MSVC TODO check ARM/ARM64 !!!
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
    # TODO more MSVC versions 2019
}

contains(DEFINES, CREATE_PDB) {
    QMAKE_CXXFLAGS += /Zi
    QMAKE_LFLAGS += /DEBUG 
}
