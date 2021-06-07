#!/bin/sh -x
export QMAKE_PATH=$HOME/Qt/5.15.2/clang_64/bin/qmake

export X_SOURCE_PATH=$PWD
export X_BUILD_NAME=die_mac_portable
export X_RELEASE_VERSION=$(cat "release_version.txt")

source build_tools/mac.sh

check_file $QMAKE_PATH

if [ -z "$X_ERROR" ]; then
    make_init
    make_build "$X_SOURCE_PATH/die_source.pro"
    cd "$X_SOURCE_PATH/gui_source"
    make_translate "gui_source_tr.pro" die
    cd "$X_SOURCE_PATH"

    check_file "$X_SOURCE_PATH/build/release/diec"
    check_file "$X_SOURCE_PATH/build/release/die.app/Contents/MacOS/die"
    if [ -z "$X_ERROR" ]; then
        cp -R "$X_SOURCE_PATH/build/release/die.app"    "$X_SOURCE_PATH/release/$X_BUILD_NAME"
        cp -R "$X_SOURCE_PATH/build/release/diec"       "$X_SOURCE_PATH/release/$X_BUILD_NAME/die.app/Contents/MacOS/"

        mkdir -p $X_SOURCE_PATH/release/$X_BUILD_NAME/die.app/Contents/Resources/signatures
        cp -R $X_SOURCE_PATH/signatures/crypto.db       $X_SOURCE_PATH/release/$X_BUILD_NAME/die.app/Contents/Resources/signatures
        cp -Rf $X_SOURCE_PATH/XStyles/qss               $X_SOURCE_PATH/release/$X_BUILD_NAME/die.app/Contents/Resources/
        cp -Rf $X_SOURCE_PATH/Detect-It-Easy/info       $X_SOURCE_PATH/release/$X_BUILD_NAME/die.app/Contents/Resources/
        cp -Rf $X_SOURCE_PATH/Detect-It-Easy/db         $X_SOURCE_PATH/release/$X_BUILD_NAME/die.app/Contents/Resources/

        fiximport "$X_SOURCE_PATH/build/release/die.app/Contents/MacOS/die"
        fiximport "$X_SOURCE_PATH/build/release/die.app/Contents/MacOS/diec"

        deploy_qt_library QtWidgets die
        deploy_qt_library QtGui die
        deploy_qt_library QtCore die
        deploy_qt_library QtDBus die
        deploy_qt_library QtPrintSupport die
        deploy_qt_library QtSvg die
        deploy_qt_library QtOpenGL die
        deploy_qt_library QtNetwork die
        deploy_qt_library QtScript die
        deploy_qt_library QtScriptTools die

        deploy_qt_plugin platforms libqcocoa die
        deploy_qt_plugin platforms libqminimal die
        deploy_qt_plugin platforms libqoffscreen die
        
        deploy_qt_plugin imageformats qjpeg die
        deploy_qt_plugin imageformats qtiff die
        deploy_qt_plugin imageformats qico die
        deploy_qt_plugin imageformats qgif die

        make_release
        make_clear
    fi
fi