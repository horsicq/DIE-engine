# --- Qt version detection (supports Qt5 and Qt6) ---
if(TARGET Qt6::Core OR TARGET Qt6::Widgets)
    set(DIE_QT_MAJOR 6)
elseif(TARGET Qt5::Core OR TARGET Qt5::Widgets)
    set(DIE_QT_MAJOR 5)
else()
    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
    set(DIE_QT_MAJOR ${QT_VERSION_MAJOR})
endif()

option(DIE_BUILD_GUI "Build GUI application" ON)
option(DIE_BUILD_LITE "Build DIE Lite application" ON)
option(DIE_BUILD_TRANSLATIONS "Build translation files" ON)
option(DIE_BUILD_UPDATER "Build die-update and database-update launchers" ON)

if(DIE_BUILD_GUI OR DIE_BUILD_LITE)
    set(DIE_QT_COMPONENTS Widgets Concurrent Network PrintSupport OpenGL Svg Sql)
else()
    set(DIE_QT_COMPONENTS Core Concurrent)
    if(DIE_BUILD_UPDATER)
        list(APPEND DIE_QT_COMPONENTS Network)
    endif()
endif()

if(DIE_QT_MAJOR EQUAL 5)
    if(NOT TARGET Qt5::Widgets)
        set(DIE_QT5_EXTRA_COMPONENTS Script)
        if(DIE_BUILD_GUI OR DIE_BUILD_LITE)
            list(APPEND DIE_QT5_EXTRA_COMPONENTS ScriptTools)
        endif()
        if(DIE_BUILD_TRANSLATIONS)
            list(APPEND DIE_QT5_EXTRA_COMPONENTS LinguistTools)
        endif()
        find_package(Qt5 REQUIRED COMPONENTS ${DIE_QT_COMPONENTS} ${DIE_QT5_EXTRA_COMPONENTS})
    endif()
elseif(DIE_QT_MAJOR GREATER_EQUAL 6)
    set(DIE_QT6_EXTRA_COMPONENTS Qml)
    if(DIE_BUILD_TRANSLATIONS)
        list(APPEND DIE_QT6_EXTRA_COMPONENTS LinguistTools)
    endif()
    find_package(Qt${DIE_QT_MAJOR} REQUIRED COMPONENTS ${DIE_QT_COMPONENTS} ${DIE_QT6_EXTRA_COMPONENTS})
    if((DIE_BUILD_GUI OR DIE_BUILD_LITE) AND NOT TARGET Qt5::Widgets)
        add_library(Qt5::Widgets INTERFACE IMPORTED)
        set_property(TARGET Qt5::Widgets PROPERTY
            INTERFACE_LINK_LIBRARIES Qt${DIE_QT_MAJOR}::Widgets)
    endif()
endif()

# --- Version from file ---
file(STRINGS "${CMAKE_CURRENT_LIST_DIR}/../release_version.txt" X_PROJECT_VERSION)
# C as well as CXX: XArchive contributes C translation units directly (the Algos
# zstd/bzip2/zlib amalgamations). Without C enabled, CMake emits no <CompileAs>
# for them, MSBuild compiles them as C by extension, and they pull in a C++ STL
# header -- "error STL1003: Unexpected compiler, expected C++ compiler".
project(DetectItEasy VERSION ${X_PROJECT_VERSION} LANGUAGES C CXX)
option(USE_DIE "Enable Detect It Easy integration" ON)

# --- Windows 7 Compatibility (sysinfo-dll fix) ---
# MSVC 2013 + Qt 5.6.3 remains the supported XP packaging path; keep its
# existing SDK defaults instead of raising that target to Windows 7.
if(WIN32 AND NOT (MSVC AND MSVC_VERSION EQUAL 1800))
    add_compile_definitions(_WIN32_WINNT=0x0601 WINVER=0x0601 NTDDI_VERSION=0x06010000)
endif()

# --- Project identity variables ---
set(X_COMPANYNAME    "ntinfo")
set(X_PROJECTNAME    "DetectItEasy")
set(X_MAINTAINER     "horsicq@gmail.com")
set(X_DESCRIPTION    "Detect It Easy is a program for determining file types")
set(X_HOMEPAGE       "http://ntinfo.biz")
set(X_ORIGINAL_FILENAME "die")

# --- macOS bundle ID ---
if(NOT DEFINED MACOSX_BUNDLE_GUI_IDENTIFIER OR "${MACOSX_BUNDLE_GUI_IDENTIFIER}" STREQUAL "")
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.ntinfo.die")
endif()
set(BUNDLE_ID_OPTION "${MACOSX_BUNDLE_GUI_IDENTIFIER}")

# --- Resource paths ---
set(X_RESOURCE_DIR               "${CMAKE_CURRENT_LIST_DIR}/../res")
set(X_WINDOWS_ICON               "${CMAKE_CURRENT_LIST_DIR}/../icons/main.ico")
set(X_WINDOWS_RESOURCE_RC_IN     "${X_RESOURCE_DIR}/windows/resource.rc.in")
set(X_WINDOWS_RESOURCE_ICON_RC_IN "${X_RESOURCE_DIR}/windows/resource_icon.rc.in")
set(X_WINDOWS_RESOURCE_RC        "${X_RESOURCE_DIR}/windows/resource.rc")
set(X_WINDOWS_RESOURCE_ICON_RC   "${X_RESOURCE_DIR}/windows/resource_icon.rc")
set(X_MACOS_ICON                 "${CMAKE_CURRENT_LIST_DIR}/../icons/main.icns")
set(X_MACOS_INFO_PLIST_IN        "${X_RESOURCE_DIR}/macos/Info.plist.in")
set(X_MACOS_INFO_PLIST           "${X_RESOURCE_DIR}/macos/Info.plist")

# --- Optional: packaging support via _mylibs ---
set(DIE_DEPLOY_INIT "${CMAKE_SOURCE_DIR}/dep/build_tools/cmake/deploy_init.cmake")
if(EXISTS "${DIE_DEPLOY_INIT}")
    include("${DIE_DEPLOY_INIT}")
else()
    message(STATUS "Skipping deploy_init.cmake — packaging templates missing")
endif()

# --- Translations ---
set(DIE_TRANSLATION_HELPER "${CMAKE_SOURCE_DIR}/dep/build_tools/cmake/deploy_translation.cmake")
if(DIE_BUILD_TRANSLATIONS AND EXISTS "${DIE_TRANSLATION_HELPER}")
    include("${DIE_TRANSLATION_HELPER}")

    set(DIE_TRANSLATION_DIR "${CMAKE_CURRENT_LIST_DIR}/../translations")
    set(DIE_TS_FILES
        "${DIE_TRANSLATION_DIR}/die_ar.ts"
        "${DIE_TRANSLATION_DIR}/die_bn.ts"
        "${DIE_TRANSLATION_DIR}/die_de.ts"
        "${DIE_TRANSLATION_DIR}/die_es.ts"
        "${DIE_TRANSLATION_DIR}/die_fa.ts"
        "${DIE_TRANSLATION_DIR}/die_fr.ts"
        "${DIE_TRANSLATION_DIR}/die_he.ts"
        "${DIE_TRANSLATION_DIR}/die_hy.ts"
        "${DIE_TRANSLATION_DIR}/die_id.ts"
        "${DIE_TRANSLATION_DIR}/die_it.ts"
        "${DIE_TRANSLATION_DIR}/die_ja.ts"
        "${DIE_TRANSLATION_DIR}/die_ko.ts"
        "${DIE_TRANSLATION_DIR}/die_pl.ts"
        "${DIE_TRANSLATION_DIR}/die_pt_BR.ts"
        "${DIE_TRANSLATION_DIR}/die_pt_PT.ts"
        "${DIE_TRANSLATION_DIR}/die_ru.ts"
        "${DIE_TRANSLATION_DIR}/die_sq.ts"
        "${DIE_TRANSLATION_DIR}/die_sv.ts"
        "${DIE_TRANSLATION_DIR}/die_tr.ts"
        "${DIE_TRANSLATION_DIR}/die_uk.ts"
        "${DIE_TRANSLATION_DIR}/die_vi.ts"
        "${DIE_TRANSLATION_DIR}/die_zh.ts"
        "${DIE_TRANSLATION_DIR}/die_zh_TW.ts"
    )

    deploy_add_translations(
        TARGET_NAME die_translations
        ADD_TO_ALL
        SOURCE_DIRS "${CMAKE_CURRENT_LIST_DIR}"
        TS_FILES ${DIE_TS_FILES}
        INSTALL_DESTINATION lang
        WINDOWS_APPDATA_SUBDIR "${X_PROJECTNAME}"
    )
endif()

# --- Library subdirectories ---
add_subdirectory("${CMAKE_SOURCE_DIR}/dep/XCppfilt"      XCppfilt)
add_subdirectory("${CMAKE_SOURCE_DIR}/dep/XCapstone"     XCapstone)
add_subdirectory("${CMAKE_SOURCE_DIR}/dep/XCapstone/x86" XCapstone_86)
add_subdirectory("${CMAKE_SOURCE_DIR}/dep/XArchive"      XArchive)
add_subdirectory("${CMAKE_SOURCE_DIR}/dep/XYara"         XYara)
add_subdirectory("${CMAKE_SOURCE_DIR}/dep/Formats/xsimd" XSIMD)

# --- Executable targets ---
add_subdirectory(updater)
if(DIE_BUILD_GUI)
    add_subdirectory(gui)
endif()
add_subdirectory(console)
if(DIE_BUILD_LITE)
    add_subdirectory(lite)
endif()