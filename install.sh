#!/bin/bash -x
set -e
export X_SOURCE_PATH=$PWD

if [ -z "$1" ]; then
    X_PREFIX="/usr"
else
    X_PREFIX="$1"
fi

mkdir -p  "$X_PREFIX/bin"
mkdir -p  "$X_PREFIX/share/applications"
mkdir -p  "$X_PREFIX/share/icons"
mkdir -p  "$X_PREFIX/share/metainfo"
mkdir -p  "$X_PREFIX/lib/die"
mkdir -p  "$X_PREFIX/lib/die/signatures"

cp -f "$X_SOURCE_PATH/build/release/die"                              "$X_PREFIX/bin/"
cp -f "$X_SOURCE_PATH/build/release/diec"                             "$X_PREFIX/bin/"
cp -f "$X_SOURCE_PATH/build/release/diel"                             "$X_PREFIX/bin/"
if [ -f "$X_SOURCE_PATH/build/release/die-update" ]; then
    cp -f "$X_SOURCE_PATH/build/release/die-update" "$X_PREFIX/lib/die/" || exit $?
    ln -sf "../lib/die/die-update" "$X_PREFIX/bin/die-update"
fi
cp -f "$X_SOURCE_PATH/LINUX/io.github.horsicq.detect-it-easy.desktop" "$X_PREFIX/share/applications/"
cp -Rf "$X_SOURCE_PATH/LINUX/hicolor/"                                "$X_PREFIX/share/icons/"
cp -f "$X_SOURCE_PATH/LINUX/io.github.horsicq.detect-it-easy.metainfo.xml" "$X_PREFIX/share/metainfo/"
cp -Rf "$X_SOURCE_PATH/dep/XStyles/qss/"                                  "$X_PREFIX/lib/die/"
cp -f "$X_SOURCE_PATH/dep/signatures/crypto.db"                           "$X_PREFIX/lib/die/signatures/"
cp -Rf "$X_SOURCE_PATH/dep/XInfoDB/info/"                                 "$X_PREFIX/lib/die/"
cp -Rf "$X_SOURCE_PATH/dep/Detect-It-Easy/db/"                            "$X_PREFIX/lib/die/"
cp -Rf "$X_SOURCE_PATH/dep/XYara/yara_rules/"                             "$X_PREFIX/lib/die/"
[ "${X_WITH_PEID:-OFF}" = "ON" ] || rm -f "$X_PREFIX/lib/die/yara_rules/peid.yar"
if [ "${X_WITH_PEID:-OFF}" = "ON" ]; then
    cp -Rf "$X_SOURCE_PATH/dep/XPEID/peid/"                              "$X_PREFIX/lib/die/"
fi
for db_extra_dir in "$X_SOURCE_PATH"/dep/Detect-It-Easy/db_extra/*/; do
    if [ -d "$db_extra_dir" ]; then
        cp -Rf "$db_extra_dir" "$X_PREFIX/lib/die/db/" || exit $?
    fi
done
cp -Rf "$X_SOURCE_PATH/dep/Detect-It-Easy/db_custom/"                     "$X_PREFIX/lib/die/"
cp -Rf "$X_SOURCE_PATH/images/"                                       "$X_PREFIX/lib/die/"
