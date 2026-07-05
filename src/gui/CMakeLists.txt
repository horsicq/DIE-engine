include(${CMAKE_CURRENT_LIST_DIR}/../../build_tools/cmake/cpp_standart_setup.cmake)

if(MSVC)
    add_compile_options(/FS)
    add_compile_options($<$<CONFIG:Debug>:/Z7> $<$<CONFIG:RelWithDebInfo>:/Z7>)
endif()

if(WIN32)
    add_compile_definitions(UNICODE _UNICODE)
endif()

add_definitions(-DUSE_DEX)
add_definitions(-DUSE_PDF)
add_definitions(-DUSE_ARCHIVE)
add_definitions(-DUSE_DIE)
add_definitions(-DUSE_XSIMD)

option(WITH_YARA "Use Yara" ON)
if(WITH_YARA)
    add_definitions(-DUSE_YARA)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/../../XMIMEWidget/xmimewidget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../FormatWidgets/formatwidgets.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../XSingleApplication/xsingleapplication.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../XAboutWidget/xaboutwidget.cmake)

if(UNIX AND NOT APPLE)
    include(GNUInstallDirs)
endif()

set(DIE_GUI_TARGET DetectItEasy)
set(DIE_GUI_SOURCES
    ${XMIMEWIDGET_SOURCES}
    ${FORMATWIDGETS_SOURCES}
    ${XSINGLEAPPLICATION_SOURCES}
    ${XABOUTWIDGET_SOURCES}
    dialogabout.cpp
    dialogabout.h
    dialogabout.ui
    dialogoptions.cpp
    dialogoptions.h
    dialogoptions.ui
    dialogselectstyle.cpp
    dialogselectstyle.h
    dialogselectstyle.ui
    desktopintegrationhelper.cpp
    desktopintegrationhelper.h
    guimainwindow.cpp
    guimainwindow.h
    guimainwindow.ui
    main_gui.cpp
    rsrc.qrc
    res.qrc
)

if(WIN32 AND EXISTS "${X_WINDOWS_RESOURCE_ICON_RC}")
    list(APPEND DIE_GUI_SOURCES "${X_WINDOWS_RESOURCE_ICON_RC}")
endif()

if(APPLE AND EXISTS "${X_MACOS_ICON}")
    set_source_files_properties("${X_MACOS_ICON}" PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources"
    )
    list(APPEND DIE_GUI_SOURCES "${X_MACOS_ICON}")
endif()

add_executable(${DIE_GUI_TARGET} WIN32
    ${DIE_GUI_SOURCES}
)

set_target_properties(${DIE_GUI_TARGET} PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE    TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER "${MACOSX_BUNDLE_GUI_IDENTIFIER}"
    OUTPUT_NAME "die"
)

target_include_directories(${DIE_GUI_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${DIE_GUI_TARGET} PRIVATE
    bzip2
    lzma
    zlib
    capstone
    cppfilt
    ppmd
    xsimd
    Qt${DIE_QT_MAJOR}::Widgets
    Qt${DIE_QT_MAJOR}::Concurrent
    Qt${DIE_QT_MAJOR}::Network
    Qt${DIE_QT_MAJOR}::PrintSupport
    Qt${DIE_QT_MAJOR}::OpenGL
    Qt${DIE_QT_MAJOR}::Svg
    Qt${DIE_QT_MAJOR}::Sql
)

if(DIE_QT_MAJOR EQUAL 5)
    target_link_libraries(${DIE_GUI_TARGET} PRIVATE
        Qt5::Script
        Qt5::ScriptTools
    )
elseif(DIE_QT_MAJOR GREATER_EQUAL 6)
    target_link_libraries(${DIE_GUI_TARGET} PRIVATE
        Qt${DIE_QT_MAJOR}::Qml
    )
endif()

if(WITH_YARA)
    target_link_libraries(${DIE_GUI_TARGET} PRIVATE yara)
endif()

if(WIN32)
    target_link_libraries(${DIE_GUI_TARGET} PRIVATE Wintrust Crypt32)
endif()

if(TARGET die_translations)
    add_dependencies(${DIE_GUI_TARGET} die_translations)
endif()

if(UNIX AND NOT APPLE)
    set(DIE_RUNTIME_DESTINATION "${CMAKE_INSTALL_BINDIR}")
    set(DIE_DOC_DESTINATION "${CMAKE_INSTALL_DOCDIR}/detect-it-easy")
    set(DIE_DESKTOP_TEMPLATE "${CMAKE_CURRENT_SOURCE_DIR}/../../res/linux/die.desktop.in")
    set(DIE_DESKTOP_FILE "${CMAKE_CURRENT_BINARY_DIR}/die.desktop")
    if(EXISTS "${DIE_DESKTOP_TEMPLATE}")
        configure_file("${DIE_DESKTOP_TEMPLATE}" "${DIE_DESKTOP_FILE}" @ONLY)
    endif()
else()
    set(DIE_RUNTIME_DESTINATION ".")
    set(DIE_DOC_DESTINATION ".")
endif()

install(TARGETS ${DIE_GUI_TARGET}
    RUNTIME DESTINATION ${DIE_RUNTIME_DESTINATION}
    BUNDLE  DESTINATION .
)

if(UNIX AND NOT APPLE)
    install(FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../README.md"
        DESTINATION ${DIE_DOC_DESTINATION}
        OPTIONAL
    )
    if(EXISTS "${DIE_DESKTOP_FILE}")
        install(FILES "${DIE_DESKTOP_FILE}"
            DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications")
    endif()
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../LINUX/hicolor/"
        DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor"
        OPTIONAL
    )
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../LINUX/io.github.horsicq.detect-it-easy.metainfo.xml"
        DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/metainfo"
        OPTIONAL
    )
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../XStyles/qss"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/die" OPTIONAL)
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../XInfoDB/info"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/die" OPTIONAL)
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../Detect-It-Easy/db"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/die" OPTIONAL)
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../XYara/yara_rules"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/die" OPTIONAL)
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../images"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/die" OPTIONAL)
else()
    install(FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE"
        DESTINATION .
        RENAME LICENSE.txt
        OPTIONAL
    )
    install(FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/../../README.md"
        "${X_WINDOWS_ICON}"
        DESTINATION .
        OPTIONAL
    )
    install(CODE [[file(WRITE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/portable" "")]])
endif()

if(APPLE)
    include(${CMAKE_CURRENT_LIST_DIR}/../../build_tools/cmake/macdeployqt.cmake)
endif()

if(WIN32)
    include(${CMAKE_CURRENT_LIST_DIR}/../../build_tools/cmake/deploy_qt_windows.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/../../build_tools/cmake/deploy_msvc.cmake)
endif()
