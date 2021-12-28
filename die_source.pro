TEMPLATE        = subdirs
SUBDIRS         += build_libs
SUBDIRS         += console_source
SUBDIRS         += gui_source
SUBDIRS         += lite_source

isEmpty(PREFIX) {
 PREFIX = /usr
}

target.path = $PWD
target.commands = bash -x "$$PWD/install.sh $$PREFIX"
INSTALLS += target


#!/bin/bash -x
export X_SOURCE_PATH=$PWD

cp -f $X_SOURCE_PATH/build/release/xelfviewer                       /usr/bin/
cp -f $X_SOURCE_PATH/LINUX/xelfviewer.desktop                      /usr/share/applications/
cp -Rf $X_SOURCE_PATH/LINUX/hicolor/                               /usr/share/icons/
cp -Rf $X_SOURCE_PATH/XStyles/qss/                                  /usr/lib/xelfviewer/
mkdir -p  /usr/lib/xelfviewer/signatures
cp -f $X_SOURCE_PATH/signatures/crypto.db                           /usr/lib/xelfviewer/signatures/
