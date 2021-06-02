TEMPLATE      = subdirs
SUBDIRS         += build_libs
SUBDIRS         += console_source
SUBDIRS         += gui_source

target.commands = sh -x $$PWD/install.sh
INSTALLS += target
