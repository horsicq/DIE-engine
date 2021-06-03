TEMPLATE        = subdirs
SUBDIRS         += build_libs
SUBDIRS         += console_source
SUBDIRS         += gui_source

target.path = $$PWD
target.commands = bash -x $$PWD/install.sh
INSTALLS += target
