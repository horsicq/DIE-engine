# --- Qt version detection (supports Qt5 and Qt6) ---
if(TARGET Qt6::Core OR TARGET Qt6::Widgets)
    set(DIE_QT_MAJOR 6)
elseif(TARGET Qt5::Core OR TARGET Qt5::Widgets)
    set(DIE_QT_MAJOR 5)
else()
    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
    set(DIE_QT_MAJOR ${QT_VERSION_MAJOR})
endif()

set(DIE_QT_COMPONENTS Widgets Concurrent Network PrintSupport OpenGL Svg Sql)

if(DIE_QT_MAJOR EQUAL 5)
    if(NOT TARGET Qt5::Widgets)
        find_package(Qt5 REQUIRED COMPONENTS ${DIE_QT_COMPONENTS} Script ScriptTools LinguistTools)
    endif()
elseif(DIE_QT_MAJOR GREATER_EQUAL 6)
    find_package(Qt${DIE_QT_MAJOR} REQUIRED COMPONENTS ${DIE_QT_COMPONENTS} Qml LinguistTools)
    if(NOT TARGET Qt5::Widgets)
        add_library(Qt5::Widgets INTERFACE IMPORTED)
        set_property(TARGET Qt5::Widgets PROPERTY
            INTERFACE_LINK_LIBRARIES Qt${DIE_QT_MAJOR}::Widgets)
    endif()
endif()

# --- Version from file ---
file(STRINGS "${CMAKE_CURRENT_LIST_DIR}/../release_version.txt" X_PROJECT_VERSION)
project(DetectItEasy VERSION ${X_PROJECT_VERSION} LANGUAGES CXX)

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
set(DIE_DEPLOY_INIT "${CMAKE_CURRENT_LIST_DIR}/../build_tools/cmake/deploy_init.cmake")
if(EXISTS "${DIE_DEPLOY_INIT}"
   AND EXISTS "${X_WINDOWS_RESOURCE_RC_IN}"
   AND EXISTS "${X_WINDOWS_RESOURCE_ICON_RC_IN}"
   AND EXISTS "${X_MACOS_ICON}"
   AND EXISTS "${X_MACOS_INFO_PLIST_IN}")
    include("${DIE_DEPLOY_INIT}")
else()
    message(STATUS "Skipping deploy_init.cmake — packaging templates missing")
endif()

# --- Translations ---
set(DIE_TRANSLATION_HELPER "${CMAKE_CURRENT_LIST_DIR}/../build_tools/cmake/deploy_translation.cmake")
if(EXISTS "${DIE_TRANSLATION_HELPER}")
    include("${DIE_TRANSLATION_HELPER}")

    set(DIE_TS_FILES
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die_ar_AR.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die_bn_BN.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die_de_DE.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die_es_ES.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die_fa_FA.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die_fr_FR.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die_he_IL.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die_id_ID.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die_it_IT.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die_ja_JP.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die_ko_KR.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die_pl_PL.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die_pt_BR.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die_pt_PT.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die_ru_RU.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die_sv_SE.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die_tr_TR.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die_uk_UA.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die_vi_VN.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die_sq_AL.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die_zh_CN.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die_zh_TW.ts"
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
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../XCppfilt"      XCppfilt)
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../XCapstone"     XCapstone)
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../XCapstone/x86" XCapstone_86)
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../XArchive"      XArchive)
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../XYara"         XYara)
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../Formats/xsimd" XSIMD)

# --- Executable targets ---
add_subdirectory(gui)
add_subdirectory(console)
add_subdirectory(lite)
