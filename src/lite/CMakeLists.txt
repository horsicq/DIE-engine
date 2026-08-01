include(${CMAKE_CURRENT_LIST_DIR}/../../build_tools/cmake/cpp_standart_setup.cmake)

if(MSVC)
    add_compile_options(/FS)
    add_compile_options($<$<CONFIG:Debug>:/Z7> $<$<CONFIG:RelWithDebInfo>:/Z7>)
endif()

add_definitions(-DUSE_DEX)
add_definitions(-DUSE_PDF)
add_definitions(-DUSE_ARCHIVE)
add_definitions(-DUSE_DIE)
add_definitions(-DUSE_XSIMD)

include(${CMAKE_CURRENT_LIST_DIR}/../../die_script/die_script.cmake)
if(NOT DEFINED XOPTIONS_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../XOptions/xoptions.cmake)
endif()
include(${CMAKE_CURRENT_LIST_DIR}/../../Controls/xcomboboxex.cmake)

if(UNIX AND NOT APPLE)
    include(GNUInstallDirs)
endif()

set(DIEL_LITE_ICON "${CMAKE_CURRENT_LIST_DIR}/../../icons/lite.icns")

set(DIEL_SOURCES
    ${DIE_SCRIPT_SOURCES}
    ${XOPTIONS_SOURCES}
    ${XCOMBOBOXEX_SOURCES}
    main_lite.cpp
    litemainwindow.cpp
    litemainwindow.h
    litemainwindow.ui
)

if(APPLE AND EXISTS "${DIEL_LITE_ICON}")
    set_source_files_properties("${DIEL_LITE_ICON}" PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources"
    )
    list(APPEND DIEL_SOURCES "${DIEL_LITE_ICON}")
endif()

if(WIN32 AND EXISTS "${X_WINDOWS_RESOURCE_ICON_RC_IN}")
    set(X_ORIGINAL_FILENAME "diel")
    set(X_WINDOWS_ICON "${CMAKE_CURRENT_LIST_DIR}/../../icons/lite.ico")
    configure_file("${X_WINDOWS_RESOURCE_ICON_RC_IN}"
                   "${CMAKE_CURRENT_BINARY_DIR}/resource_diel.rc" @ONLY)
    configure_file("${X_RESOURCE_DIR}/windows/windows.manifest.xml"
                   "${CMAKE_CURRENT_BINARY_DIR}/windows.manifest.xml" COPYONLY)
    list(APPEND DIEL_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/resource_diel.rc")
endif()

add_executable(DetectItEasyLite WIN32
    ${DIEL_SOURCES}
)

set_target_properties(DetectItEasyLite PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE    TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.ntinfo.diel"
    OUTPUT_NAME "diel"
)

target_include_directories(DetectItEasyLite PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(DetectItEasyLite PRIVATE
    bzip2
    lzma
    zlib
    capstone_x86
    ppmd
    xsimd
    Qt${DIE_QT_MAJOR}::Widgets
    Qt${DIE_QT_MAJOR}::Concurrent
)

if(DIE_QT_MAJOR EQUAL 5)
    target_link_libraries(DetectItEasyLite PRIVATE Qt5::Script)
elseif(DIE_QT_MAJOR GREATER_EQUAL 6)
    target_link_libraries(DetectItEasyLite PRIVATE Qt${DIE_QT_MAJOR}::Qml)
endif()

if(WIN32)
    target_link_libraries(DetectItEasyLite PRIVATE Wintrust Crypt32)
endif()

if(TARGET die_translations)
    add_dependencies(DetectItEasyLite die_translations)
endif()

if(UNIX AND NOT APPLE)
    set(DIEL_RUNTIME_DESTINATION "${CMAKE_INSTALL_BINDIR}")
else()
    set(DIEL_RUNTIME_DESTINATION ".")
endif()

install(TARGETS DetectItEasyLite
    RUNTIME DESTINATION ${DIEL_RUNTIME_DESTINATION}
    BUNDLE  DESTINATION .
)

if(APPLE)
    include(${CMAKE_CURRENT_LIST_DIR}/../../build_tools/cmake/macdeployqt.cmake)
endif()

if(WIN32)
    include(${CMAKE_CURRENT_LIST_DIR}/../../build_tools/cmake/deploy_qt_windows.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/../../build_tools/cmake/deploy_msvc.cmake)
endif()
