INCLUDEPATH += $$PWD/src
DEPENDPATH += $$PWD/src

win32 {
    CONFIG(debug, debug|release) {
        LIBS += $$PWD/libs/win32/$$QT_MAJOR_VERSION/$$QT_MINOR_VERSION/qwtd.lib
    } else {
        LIBS += $$PWD/libs/win32/$$QT_MAJOR_VERSION/$$QT_MINOR_VERSION/qwt.lib
    }
}

message($$LIBS)

unix:!macx {
	BITSIZE = $$system(getconf LONG_BIT)
	if (contains(BITSIZE, 64)) {
                LIBS +=  $$PWD/libs/lin64/$$QT_MAJOR_VERSION/$$QT_MINOR_VERSION/libqwt.a
	}
	if (contains(BITSIZE, 32)) {
		# TODO
	}
}

unix:macx {
LIBS +=  $$PWD/libs/mac/$$QT_MAJOR_VERSION/$$QT_MINOR_VERSION/libqwt.a
}

