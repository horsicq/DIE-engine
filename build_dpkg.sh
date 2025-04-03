#!/bin/bash -x
export QMAKE_PATH=/usr/bin/qmake

# Enable 'set -e' to ensure the script exits immediately if any command returns a non-zero exit code.
# This is particularly useful so that github can correctly indicate the status of the process!
set -e

export X_SOURCE_PATH=$PWD
export X_BUILD_NAME=die_linux_portable
export X_RELEASE_VERSION=$(cat "release_version.txt")

source build_tools/linux.sh

check_file $QMAKE_PATH

if [ -z "$X_ERROR" ]; then
    make_init
    make_build "$X_SOURCE_PATH/die_source.pro"
    cd "$X_SOURCE_PATH/gui_source"
    make_translate "gui_source_tr.pro"
    cd "$X_SOURCE_PATH"

    check_file "$X_SOURCE_PATH/build/release/die"
    check_file "$X_SOURCE_PATH/build/release/diec"
    check_file "$X_SOURCE_PATH/build/release/diel"
    if [ -z "$X_ERROR" ]; then
        create_deb_app_dir die
        
        export X_PACKAGENAME='detectiteasy'
        export X_PRIORITY='optional'
        export X_SECTION='devel'
        export X_MAINTAINER='hors <horsicq@gmail.com>'
        
        export X_HOMEPAGE='http://ntinfo.biz'
        export X_DESCRIPTION='Detect It Easy is a program for determining types of files.'
        
        BASE_DEPENDS='libqt5core5a, libqt5svg5, libqt5gui5, libqt5widgets5, libqt5opengl5, libqt5dbus5, libqt5scripttools5, libqt5script5, libqt5network5, libqt5sql5'
        
        export X_DEPENDS="$BASE_DEPENDS"
        
        create_deb_control $X_SOURCE_PATH/release/$X_BUILD_NAME/DEBIAN/control
        
        #cp -f $X_SOURCE_PATH/LICENSE                                        $X_SOURCE_PATH/release/$X_BUILD_NAME/
        
        cp -f $X_SOURCE_PATH/build/release/die                              $X_SOURCE_PATH/release/$X_BUILD_NAME/usr/bin/
        cp -f $X_SOURCE_PATH/build/release/diec                             $X_SOURCE_PATH/release/$X_BUILD_NAME/usr/bin/
        cp -f $X_SOURCE_PATH/build/release/diel                             $X_SOURCE_PATH/release/$X_BUILD_NAME/usr/bin/
        cp -f $X_SOURCE_PATH/LINUX/io.github.horsicq.detect-it-easy.desktop $X_SOURCE_PATH/release/$X_BUILD_NAME/usr/share/applications/
        cp -Rf $X_SOURCE_PATH/LINUX/hicolor/                                $X_SOURCE_PATH/release/$X_BUILD_NAME/usr/share/icons/
        cp -f $X_SOURCE_PATH/LINUX/io.github.horsicq.detect-it-easy.metainfo.xml $X_SOURCE_PATH/release/$X_BUILD_NAME/usr/share/metainfo/
        cp -Rf $X_SOURCE_PATH/XStyles/qss/                                  $X_SOURCE_PATH/release/$X_BUILD_NAME/usr/lib/die/
        cp -Rf $X_SOURCE_PATH/XInfoDB/info/                                 $X_SOURCE_PATH/release/$X_BUILD_NAME/usr/lib/die/
        cp -Rf $X_SOURCE_PATH/Detect-It-Easy/db/                            $X_SOURCE_PATH/release/$X_BUILD_NAME/usr/lib/die/
		cp -Rf $X_SOURCE_PATH/Detect-It-Easy/db_custom/                     $X_SOURCE_PATH/release/$X_BUILD_NAME/usr/lib/die/
        cp -Rf $X_SOURCE_PATH/XYara/yara_rules/                             $X_SOURCE_PATH/release/$X_BUILD_NAME/usr/lib/die/
        mkdir -p $X_SOURCE_PATH/release/$X_BUILD_NAME/usr/lib/die/lang/
        cp -f $X_SOURCE_PATH/gui_source/translation/*.qm                    $X_SOURCE_PATH/release/$X_BUILD_NAME/usr/lib/die/lang/
        mkdir -p $X_SOURCE_PATH/release/$X_BUILD_NAME/usr/lib/die/signatures
        cp -f $X_SOURCE_PATH/signatures/crypto.db                           $X_SOURCE_PATH/release/$X_BUILD_NAME/usr/lib/die/signatures/
        cp -Rf $X_SOURCE_PATH/images                                        $X_SOURCE_PATH/release/$X_BUILD_NAME/usr/lib/die/

        make_deb
        mv $X_SOURCE_PATH/release/$X_BUILD_NAME.deb $X_SOURCE_PATH/release/die_${X_RELEASE_VERSION}_${X_OS_VERSION}_${X_ARCHITECTURE}.deb
        make_clear
    fi
fi
