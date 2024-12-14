#!/bin/bash -x
export X_SOURCE_PATH=$PWD

if [ -z "$1" ]; then
    X_PREFIX="/usr"
else
    X_PREFIX="$1"
fi

mkdir -p  $X_PREFIX/bin
mkdir -p  $X_PREFIX/share/applications
mkdir -p  $X_PREFIX/share/icons
mkdir -p  $X_PREFIX/share/metainfo
mkdir -p  $X_PREFIX/lib/die
mkdir -p  $X_PREFIX/lib/die/signatures

cp -f $X_SOURCE_PATH/build/release/die                              $X_PREFIX/bin/
cp -f $X_SOURCE_PATH/build/release/diec                             $X_PREFIX/bin/
cp -f $X_SOURCE_PATH/build/release/diel                             $X_PREFIX/bin/
cp -f $X_SOURCE_PATH/LINUX/io.github.horsicq.detect-it-easy.desktop $X_PREFIX/share/applications/
cp -Rf $X_SOURCE_PATH/LINUX/hicolor/                                $X_PREFIX/share/icons/
cp -f $X_SOURCE_PATH/LINUX/io.github.horsicq.detect-it-easy.metainfo.xml $X_PREFIX/share/metainfo/
cp -Rf $X_SOURCE_PATH/XStyles/qss/                                  $X_PREFIX/lib/die/
cp -f $X_SOURCE_PATH/signatures/crypto.db                           $X_PREFIX/lib/die/signatures/
cp -Rf $X_SOURCE_PATH/XInfoDB/info/                                 $X_PREFIX/lib/die/
cp -Rf $X_SOURCE_PATH/Detect-It-Easy/db/                            $X_PREFIX/lib/die/
cp -Rf $X_SOURCE_PATH/Detect-It-Easy/db_custom/                     $X_PREFIX/lib/die/
cp -Rf $X_SOURCE_PATH/images/                                       $X_PREFIX/lib/die/
