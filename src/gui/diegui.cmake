# Source list for the Detect It Easy GUI.
#
# Shared by the application target (src/gui/CMakeLists.txt) and the widget test
# suite (tests/CMakeLists.txt) so the two cannot drift apart: a widget added to the
# GUI is compiled into the tests without a second list to remember.
#
# Deliberately NOT part of this list:
#   * main_gui.cpp - a test executable brings its own main().
#   * rsrc.qrc / res.qrc - each target compiles the resources itself. Qt only
#     auto-initialises resources linked into an executable; routed through a static
#     library they need an explicit Q_INIT_RESOURCE and would silently come up empty.
#   * the Windows .rc and the macOS .icns, which are packaging inputs of the
#     application bundle, not of the widget code.

include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_SOURCE_DIR}/dep/XMIMEWidget/xmimewidget.cmake)
include(${CMAKE_SOURCE_DIR}/dep/FormatWidgets/formatwidgets.cmake)
include(${CMAKE_SOURCE_DIR}/dep/XSingleApplication/xsingleapplication.cmake)
include(${CMAKE_SOURCE_DIR}/dep/XAboutWidget/xaboutwidget.cmake)

set(DIEGUI_SOURCES
    ${XMIMEWIDGET_SOURCES}
    ${FORMATWIDGETS_SOURCES}
    ${XSINGLEAPPLICATION_SOURCES}
    ${XABOUTWIDGET_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/dialogabout.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogabout.h
    ${CMAKE_CURRENT_LIST_DIR}/dialogabout.ui
    ${CMAKE_CURRENT_LIST_DIR}/dialogoptions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogoptions.h
    ${CMAKE_CURRENT_LIST_DIR}/dialogoptions.ui
    ${CMAKE_CURRENT_LIST_DIR}/dialogselectstyle.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogselectstyle.h
    ${CMAKE_CURRENT_LIST_DIR}/dialogselectstyle.ui
    ${CMAKE_CURRENT_LIST_DIR}/desktopintegrationhelper.cpp
    ${CMAKE_CURRENT_LIST_DIR}/desktopintegrationhelper.h
    ${CMAKE_CURRENT_LIST_DIR}/guimainwindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/guimainwindow.h
    ${CMAKE_CURRENT_LIST_DIR}/guimainwindow.ui
)
