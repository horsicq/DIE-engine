#!/bin/sh -x

QT_BASE_PATH="$HOME/Qt"
QMAKE_PATH=""

for dir in "$QT_BASE_PATH"/5.*; do
    if [ -x "$dir/clang_64/bin/qmake" ]; then
        QMAKE_PATH="$dir/clang_64/bin/qmake"
        break
    fi
done

if [ -z "$QMAKE_PATH" ]; then
    echo "Qt 5.x version not found. Please ensure it is installed under $QT_BASE_PATH."
    exit 1
fi

export QMAKE_PATH

# Enable 'set -e' to ensure the script exits immediately if any command returns a non-zero exit code.
# This is particularly useful so that github can correctly indicate the status of the process!
set -e

export X_SOURCE_PATH=$PWD
export X_BUILD_NAME=die_mac_portable
export X_RELEASE_VERSION=$(cat "release_version.txt")

source build_tools/mac.sh

check_file "$QMAKE_PATH"

if [ -z "$X_ERROR" ]; then
    make_init
    make_build "$X_SOURCE_PATH/die_source.pro"
    cd "$X_SOURCE_PATH/gui_source"
    make_translate "gui_source_tr.pro" die
    cd "$X_SOURCE_PATH"

    check_file "$X_SOURCE_PATH/build/release/DiE.app/Contents/MacOS/DiE"
    if [ -z "$X_ERROR" ]; then
        cp -R "$X_SOURCE_PATH/build/release/DiE.app"        "$X_SOURCE_PATH/release/$X_BUILD_NAME"
        cp -R "$X_SOURCE_PATH/build/release/diec"           "$X_SOURCE_PATH/release/$X_BUILD_NAME/DiE.app/Contents/MacOS/"
        mkdir -p $X_SOURCE_PATH/release/$X_BUILD_NAME/DiE.app/Contents/Resources/signatures
        cp -R $X_SOURCE_PATH/signatures/crypto.db           $X_SOURCE_PATH/release/$X_BUILD_NAME/DiE.app/Contents/Resources/signatures/
        cp -Rf $X_SOURCE_PATH/XStyles/qss                   $X_SOURCE_PATH/release/$X_BUILD_NAME/DiE.app/Contents/Resources/
        cp -Rf $X_SOURCE_PATH/XInfoDB/info                  $X_SOURCE_PATH/release/$X_BUILD_NAME/DiE.app/Contents/Resources/
        cp -Rf $X_SOURCE_PATH/Detect-It-Easy/db             $X_SOURCE_PATH/release/$X_BUILD_NAME/DiE.app/Contents/Resources/
        cp -Rf $X_SOURCE_PATH/Detect-It-Easy/db_custom      $X_SOURCE_PATH/release/$X_BUILD_NAME/DiE.app/Contents/Resources/
        cp -Rf $X_SOURCE_PATH/images                        $X_SOURCE_PATH/release/$X_BUILD_NAME/DiE.app/Contents/Resources/
        cp -Rf $X_SOURCE_PATH/XYara/yara_rules              $X_SOURCE_PATH/release/$X_BUILD_NAME/DiE.app/Contents/Resources/
        
        deploy_qt DiE

        make_release DiE
        make_clear
    fi
fi