#!/bin/bash -x
export QMAKE_PATH=/usr/bin/qmake

export X_SOURCE_PATH=$PWD
export X_BUILD_NAME=die_linux_portable
export X_RELEASE_VERSION=$(cat "release_version.txt")

source build_tools/linux.sh

check_file $QMAKE_PATH

if [ -z "$X_ERROR" ]; then
    make_init
    
    if [ ! -f "$X_SOURCE_PATH/build/release/die" ]; then
        make_build "$X_SOURCE_PATH/die_source.pro"
        cd "$X_SOURCE_PATH/gui_source"
        make_translate "gui_source_tr.pro"
        cd "$X_SOURCE_PATH"
    fi
    
    check_file "$X_SOURCE_PATH/build/release/die"
    check_file "$X_SOURCE_PATH/build/release/diec"
    check_file "$X_SOURCE_PATH/build/release/diel"
    if [ -z "$X_ERROR" ]; then
        mkdir -p $X_SOURCE_PATH/release/$X_BUILD_NAME/base
        mkdir -p $X_SOURCE_PATH/release/$X_BUILD_NAME/base/platforms
        mkdir -p $X_SOURCE_PATH/release/$X_BUILD_NAME/base/sqldrivers
    
        cp -f $X_SOURCE_PATH/build/release/die                              $X_SOURCE_PATH/release/$X_BUILD_NAME/base/
        cp -f $X_SOURCE_PATH/build/release/diec                             $X_SOURCE_PATH/release/$X_BUILD_NAME/base/
        cp -f $X_SOURCE_PATH/build/release/diel                             $X_SOURCE_PATH/release/$X_BUILD_NAME/base/
        
        cp -R $X_QT_INSTALL_PLUGINS/platforms/libqxcb.so                    $X_SOURCE_PATH/release/$X_BUILD_NAME/base/platforms/
        cp -R $X_QT_INSTALL_PLUGINS/sqldrivers/qsqlite.dll                  $X_SOURCE_PATH/release/$X_BUILD_NAME/base/sqldrivers/
        
        cp -Lr $X_QT_INSTALL_LIBS/libQt5Core.so.5                           $X_SOURCE_PATH/release/$X_BUILD_NAME/base/
        cp -Lr $X_QT_INSTALL_LIBS/libQt5Svg.so.5                            $X_SOURCE_PATH/release/$X_BUILD_NAME/base/
        cp -Lr $X_QT_INSTALL_LIBS/libQt5Gui.so.5                            $X_SOURCE_PATH/release/$X_BUILD_NAME/base/
        cp -Lr $X_QT_INSTALL_LIBS/libQt5Sql.so.5                            $X_SOURCE_PATH/release/$X_BUILD_NAME/base/
        cp -Lr $X_QT_INSTALL_LIBS/libQt5Widgets.so.5                        $X_SOURCE_PATH/release/$X_BUILD_NAME/base/
        cp -Lr $X_QT_INSTALL_LIBS/libQt5OpenGL.so.5                         $X_SOURCE_PATH/release/$X_BUILD_NAME/base/
        cp -Lr $X_QT_INSTALL_LIBS/libQt5DBus.so.5                           $X_SOURCE_PATH/release/$X_BUILD_NAME/base/
        cp -Lr $X_QT_INSTALL_LIBS/libQt5XcbQpa.so.5                         $X_SOURCE_PATH/release/$X_BUILD_NAME/base/
        cp -Lr $X_QT_INSTALL_LIBS/libQt5ScriptTools.so.5                    $X_SOURCE_PATH/release/$X_BUILD_NAME/base/
        cp -Lr $X_QT_INSTALL_LIBS/libQt5Script.so.5                         $X_SOURCE_PATH/release/$X_BUILD_NAME/base/
        cp -Lr $X_QT_INSTALL_LIBS/libQt5Network.so.5                        $X_SOURCE_PATH/release/$X_BUILD_NAME/base/

        cp -Rf $X_SOURCE_PATH/XStyles/qss/                                  $X_SOURCE_PATH/release/$X_BUILD_NAME/base/
        cp -Rf $X_SOURCE_PATH/XInfoDB/info/                                 $X_SOURCE_PATH/release/$X_BUILD_NAME/base/
        cp -Rf $X_SOURCE_PATH/Detect-It-Easy/db/                            $X_SOURCE_PATH/release/$X_BUILD_NAME/base/
		cp -Rf $X_SOURCE_PATH/Detect-It-Easy/db_custom/                     $X_SOURCE_PATH/release/$X_BUILD_NAME/base/
        cp -Rf $X_SOURCE_PATH/XYara/yara_rules/                             $X_SOURCE_PATH/release/$X_BUILD_NAME/base/
        mkdir -p $X_SOURCE_PATH/release/$X_BUILD_NAME/base/lang/
        cp -f $X_SOURCE_PATH/gui_source/translation/*.qm                    $X_SOURCE_PATH/release/$X_BUILD_NAME/base/lang/
        mkdir -p $X_SOURCE_PATH/release/$X_BUILD_NAME/base/signatures/
        cp -f $X_SOURCE_PATH/signatures/crypto.db                           $X_SOURCE_PATH/release/$X_BUILD_NAME/base/signatures/
        cp -Rf $X_SOURCE_PATH/images                                        $X_SOURCE_PATH/release/$X_BUILD_NAME/base/
        
        create_run_shell $X_SOURCE_PATH/release/$X_BUILD_NAME/die.sh die
        create_run_shell $X_SOURCE_PATH/release/$X_BUILD_NAME/diec.sh diec
        create_run_shell $X_SOURCE_PATH/release/$X_BUILD_NAME/diel.sh diel

        cd $X_SOURCE_PATH/release
        make_targz $X_BUILD_NAME $X_SOURCE_PATH/release/die_${X_RELEASE_VERSION}_portable_${X_OS_VERSION}_${X_ARCHITECTURE}
        cd $X_SOURCE_PATH

        make_clear
    fi
fi
