# For additional build parameters
CONFIG(debug, debug|release) {
    DESTDIR = ../build/debug
} else {
    DESTDIR = ../build/release
}


!contains(QMAKE_TARGET.arch, x86_64) {
    win32-msvc2013{
       # Windows XP
       QMAKE_CXXFLAGS  += /D_USING_V110_SDK71_
       QMAKE_LFLAGS += /SUBSYSTEM:WINDOWS,5.01
    }
    win32-msvc2017{
       # Windows XP support
       QMAKE_CXXFLAGS  += /D_USING_V110_SDK71_
       QMAKE_LFLAGS += /SUBSYSTEM:WINDOWS,5.01
    }
}
