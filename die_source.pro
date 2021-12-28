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
