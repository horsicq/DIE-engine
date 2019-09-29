QT       += core gui xml script network scripttools svg concurrent printsupport

TARGET = die

TEMPLATE = app

win32 {
    RC_ICONS = pics/main.ico
}

macx {
    ICON = pics/main.icns
}

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

INCLUDEPATH += $$PWD/../
INCLUDEPATH += $$PWD/../../

DEFINES += "USE_NFD"

greaterThan(QT_MAJOR_VERSION, 4): DEFINES += "USE_YARA"

RESOURCES += \
    res.qrc

HEADERS += \
    threadplugin.h \
    threadimport.h \
    threadexport.h \
    threadentropy.h \
    threaddirectory.h \
    qdialogx.h \
    mainwindow.h \
    dialogpeversioninfo.h \
    dialogpetimedatestamp.h \
    dialogpesubsystem.h \
    dialogpestub.h \
    dialogselectplugin.h \
    dialogpesections.h \
    dialogpesectionheader.h \
    dialogpesectioncharacteristics.h \
    dialogperesource.h \
    dialogpe.h \
    dialogpeoverlay.h \
    dialogoptions.h \
    dialogpentheaders.h \
    dialogmsdosheader.h \
    dialogpemetadatatables.h \
    dialogpemetadata.h \
    dialogpemanifest.h \
    dialogpemagic.h \
    dialogpemachine.h \
    dialoginfo.h \
    dialogpeimportlibrary.h \
    dialogpeimportfunction.h \
    dialogpeimport.h \
    dialoghde.h \
    dialogextra.h \
    dialogpeexportfunction.h \
    dialogpeexport.h \
    dialogentropy.h \
    dialogelfsht.h \
    dialogelfsectionheader.h \
    dialogelfprogramheader.h \
    dialogelfpht.h \
    dialogelfheader.h \
    dialogpedosheader.h \
    dialogpedllcharacteristics.h \
    dialogdirectoryheuristicscan.h \
    dialogpedirectories.h \
    dialogpecom.h \
    dialogpechecksum.h \
    dialogpecharacteristics.h \
    dialogabout.h \
    dialogsignatures.h \
    dialognewsignature.h \
    dialogpetls.h \
    dialogpeloadconfig.h \
    dialogpeboundimport.h \
    threadsearchplugin.h \
    qtexteditxeditor.h \
    dialogpedebug.h \
    dialogpemetadatastrings.h \
    dialogpemetadataus.h \
    dialogelfclass.h \
    dialogelfdata.h \
    dialogelftype.h \
    dialogelfmachine.h \
    handleoptions.h \
    dialogmachheader.h \
    dialogmachlc.h \
    dialogmachsegments.h \
    dialogmachsections.h \
    dialogelfstrings.h \
    dialogmachuuid.h \
    dialogmachlibs.h \
    dialogpebasereloc.h \
    qxdialogx.h \
    dialogelfgeneric.h \
    dialogmachgeneric.h \
    dialogpegeneric.h \
    dialogmsdosgeneric.h \
    formelf.h \
    formpe.h \
    formmsdos.h \
    formmach.h \
    formgeneric.h \
    dialogsearch.h \
    threadsearch.h \
    searchtablemodel.h \
    dialogflc.h \
    ../_options.h \
    ../scriptpe.h \
    ../scriptmsdos.h \
    ../scriptelf.h \
    ../scriptbinary.h \
    ../scripttext.h \
    ../scriptmach.h \
    ../scan.h \
    ../scriptgeneric.h \
    ../scriptdie.h \
    ../utils.h \
    ../../pluginsscript.h \
    ../../qtreewidgetx.h \
    ../../qtexteditx.h \
    ../../qtabwidgetx.h \
    ../../qtablewidgetx.h \
    ../../qpushbuttonx.h \
    ../../qprogressbarx.h \
    ../../qlistwidgetx.h \
    ../../qlineeditx.h \
    ../../qlabelx.h \
    ../../qdatetimeeditx.h \
    ../../qcomboboxx.h \
    ../../qcheckboxx.h \
    ../../pefile.h \
    ../../msdosfile.h \
    ../../elffile.h \
    ../../textfile.h \
    ../../binary.h \
    ../../versionresource.h \
    ../../qxhexviewx.h \
    ../../qxhexview.h \
    ../../qxhexgoto.h \
    ../../qxhexform.h \
    ../../qxhexsignature.h \
    ../../qxhexsearch.h \
    ../../qradiobuttonx.h \
    ../../qxhexsearchthread.h \
    ../../qxhex.h \
    ../../qtexteditxhex.h \
    ../../qscriptenginedebuggerx.h \
    ../../qxhexdumpthread.h \
    ../../machfile.h \
    ../../scriptengineeventfilter.h

SOURCES += \
    threadplugin.cpp \
    threadimport.cpp \
    threadexport.cpp \
    threadentropy.cpp \
    threaddirectory.cpp \
    qdialogx.cpp \
    mainwindow.cpp \
    main.cpp \
    dialogpeversioninfo.cpp \
    dialogpetimedatestamp.cpp \
    dialogpesubsystem.cpp \
    dialogpestub.cpp \
    dialogselectplugin.cpp \
    dialogpesections.cpp \
    dialogpesectionheader.cpp \
    dialogpesectioncharacteristics.cpp \
    dialogperesource.cpp \
    dialogpe.cpp \
    dialogpeoverlay.cpp \
    dialogoptions.cpp \
    dialogpentheaders.cpp \
    dialogmsdosheader.cpp \
    dialogpemetadatatables.cpp \
    dialogpemetadata.cpp \
    dialogpemanifest.cpp \
    dialogpemagic.cpp \
    dialogpemachine.cpp \
    dialoginfo.cpp \
    dialogpeimportlibrary.cpp \
    dialogpeimportfunction.cpp \
    dialogpeimport.cpp \
    dialoghde.cpp \
    dialogextra.cpp \
    dialogpeexportfunction.cpp \
    dialogpeexport.cpp \
    dialogentropy.cpp \
    dialogelfsht.cpp \
    dialogelfsectionheader.cpp \
    dialogelfprogramheader.cpp \
    dialogelfpht.cpp \
    dialogelfheader.cpp \
    dialogpedosheader.cpp \
    dialogpedllcharacteristics.cpp \
    dialogdirectoryheuristicscan.cpp \
    dialogpedirectories.cpp \
    dialogpecom.cpp \
    dialogpechecksum.cpp \
    dialogpecharacteristics.cpp \
    dialogabout.cpp \
    dialogsignatures.cpp \
    dialognewsignature.cpp \
    dialogpetls.cpp \
    dialogpeloadconfig.cpp \
    dialogpeboundimport.cpp \
    threadsearchplugin.cpp \
    qtexteditxeditor.cpp \
    dialogpedebug.cpp \
    dialogpemetadatastrings.cpp \
    dialogpemetadataus.cpp \
    dialogelfclass.cpp \
    dialogelfdata.cpp \
    dialogelftype.cpp \
    dialogelfmachine.cpp \
    handleoptions.cpp \
    dialogmachheader.cpp \
    dialogmachlc.cpp \
    dialogmachsegments.cpp \
    dialogmachsections.cpp \
    dialogelfstrings.cpp \
    dialogmachuuid.cpp \
    dialogmachlibs.cpp \
    dialogpebasereloc.cpp \
    qxdialogx.cpp \
    dialogelfgeneric.cpp \
    dialogmachgeneric.cpp \
    dialogpegeneric.cpp \
    dialogmsdosgeneric.cpp \
    formgeneric.cpp \
    formpe.cpp \
    formelf.cpp \
    formmach.cpp \
    formmsdos.cpp \
    dialogsearch.cpp \
    threadsearch.cpp \
    searchtablemodel.cpp \
    dialogflc.cpp \
    ../scriptpe.cpp \
    ../scriptmsdos.cpp \
    ../scriptelf.cpp \
    ../scriptbinary.cpp \
    ../scripttext.cpp \
    ../scriptmach.cpp \
    ../scriptdie.cpp \
    ../scriptgeneric.cpp \
    ../utils.cpp \
    ../scan.cpp \
    ../../qtreewidgetx.cpp \
    ../../qtexteditx.cpp \
    ../../qtabwidgetx.cpp \
    ../../qtablewidgetx.cpp \
    ../../qpushbuttonx.cpp \
    ../../qprogressbarx.cpp \
    ../../qlistwidgetx.cpp \
    ../../qlineeditx.cpp \
    ../../qlabelx.cpp \
    ../../qdatetimeeditx.cpp \
    ../../qcomboboxx.cpp \
    ../../qcheckboxx.cpp \
    ../../pluginsscript.cpp \
    ../../pefile.cpp \
    ../../msdosfile.cpp \
    ../../elffile.cpp \
    ../../textfile.cpp \
    ../../binary.cpp \
    ../../machfile.cpp \
    ../../versionresource.cpp \
    ../../qxhexviewx.cpp \
    ../../qxhexview.cpp \
    ../../qxhexgoto.cpp \
    ../../qxhexform.cpp \
    ../../qxhexsignature.cpp \
    ../../qxhexsearch.cpp \
    ../../qradiobuttonx.cpp \
    ../../qxhexsearchthread.cpp \
    ../../qtexteditxhex.cpp \
    ../../qxhex.cpp \
    ../../qscriptenginedebuggerx.cpp \
    ../../qxhexdumpthread.cpp \
    ../../scriptengineeventfilter.cpp

FORMS += \
    mainwindow.ui \
    dialogpeversioninfo.ui \
    dialogpetimedatestamp.ui \
    dialogpesubsystem.ui \
    dialogpestub.ui \
    dialogselectplugin.ui \
    dialogpesections.ui \
    dialogpesectionheader.ui \
    dialogpesectioncharacteristics.ui \
    dialogperesource.ui \
    dialogpe.ui \
    dialogpeoverlay.ui \
    dialogoptions.ui \
    dialogpentheaders.ui \
    dialogmsdosheader.ui \
    dialogpemetadatatables.ui \
    dialogpemetadata.ui \
    dialogpemanifest.ui \
    dialogpemagic.ui \
    dialogpemachine.ui \
    dialoginfo.ui \
    dialogpeimportlibrary.ui \
    dialogpeimportfunction.ui \
    dialogpeimport.ui \
    dialoghde.ui \
    dialogextra.ui \
    dialogpeexportfunction.ui \
    dialogpeexport.ui \
    dialogentropy.ui \
    dialogelfsht.ui \
    dialogelfsectionheader.ui \
    dialogelfprogramheader.ui \
    dialogelfpht.ui \
    dialogelfheader.ui \
    dialogpedosheader.ui \
    dialogpedllcharacteristics.ui \
    dialogdirectoryheuristicscan.ui \
    dialogpedirectories.ui \
    dialogpecom.ui \
    dialogpechecksum.ui \
    dialogpecharacteristics.ui \
    dialogabout.ui \
    ../../qxhexgoto.ui \
    ../../qxhexform.ui \
    dialogsignatures.ui \
    dialognewsignature.ui \
    dialogpetls.ui \
    dialogpeloadconfig.ui \
    dialogpeboundimport.ui \
    dialogpedebug.ui \
    dialogpemetadatastrings.ui \
    dialogpemetadataus.ui \
    dialogelfclass.ui \
    dialogelfdata.ui \
    dialogelftype.ui \
    dialogelfmachine.ui \
    ../../qxhexsearch.ui \
    ../../qxhexsignature.ui \
    dialogmachheader.ui \
    dialogmachlc.ui \
    dialogmachsegments.ui \
    dialogmachsections.ui \
    dialogelfstrings.ui \
    dialogmachuuid.ui \
    dialogmachlibs.ui \
    dialogpebasereloc.ui \
    formelf.ui \
    formmach.ui \
    formmsdos.ui \
    formpe.ui \
    dialogsearch.ui \
    dialogflc.ui

TRANSLATIONS=   ../translate/*.ts

include(../../qwt/qwt.pri)
include(../../beaengine/beaengine.pri)
include(../../qtsingleapplication/qtsingleapplication.pri)
include(../../build.pri)
include(../../StaticScan/staticscan.pri)
greaterThan(QT_MAJOR_VERSION, 4): include(../../QYara/qyara.pri)

macx {
     LIBS += "-framework CoreFoundation"
}
