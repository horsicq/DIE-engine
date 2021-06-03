#!/bin/bash -x
export X_SOURCE_PATH=$PWD

cp -f $X_SOURCE_PATH/build/release/die                              /usr/bin/
cp -f $X_SOURCE_PATH/build/release/diec                             /usr/bin/
cp -f $X_SOURCE_PATH/DEBIAN/die.desktop                             /usr/share/applications/
cp -Rf $X_SOURCE_PATH/DEBIAN/hicolor/                               /usr/share/icons/
cp -Rf $X_SOURCE_PATH/XStyles/qss/                                  /usr/lib/die/
cp -Rf $X_SOURCE_PATH/Detect-It-Easy/info/                          /usr/lib/die/
cp -Rf $X_SOURCE_PATH/Detect-It-Easy/db/                            /usr/lib/die/
mkdir -p  /usr/lib/die/signatures
cp -f $X_SOURCE_PATH/signatures/crypto.db                           /usr/lib/die/signatures/
