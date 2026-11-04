# The console and GUI wrappers are Qt code, but the updater itself is the
# exact C core used by cdie.  Keeping one implementation makes archive safety,
# recovery, exit codes and messages identical in both products.
add_library(die_updater_qt STATIC dieupdater.cpp dieupdater.h)
target_include_directories(die_updater_qt PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(die_updater_qt PUBLIC Qt${DIE_QT_MAJOR}::Core)
target_compile_features(die_updater_qt PUBLIC cxx_std_11)

option(DIE_BUILD_UPDATER "Build die-update and database-update launchers" ON)
if(NOT DIE_BUILD_UPDATER)
    return()
endif()

set(DIE_CDIE_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../cdie_source" CACHE PATH
    "cdie source tree that supplies the shared database updater")
get_filename_component(DIE_CDIE_SOURCE_DIR "${DIE_CDIE_SOURCE_DIR}" ABSOLUTE)
if(NOT EXISTS "${DIE_CDIE_SOURCE_DIR}/src/updater/cdie_updater.c")
    message(FATAL_ERROR "Qt DIE needs cdie's shared updater; set DIE_CDIE_SOURCE_DIR")
endif()

set(_die_mylibs "${CMAKE_CURRENT_LIST_DIR}/../../../_mylibs")
if(NOT TARGET xxfclib::die_engine)
    set(XXFC_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(XXFC_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory("${_die_mylibs}/xxfclib" "${CMAKE_CURRENT_BINARY_DIR}/xxfclib" EXCLUDE_FROM_ALL)
endif()
if(NOT TARGET xxgithub::xxgithub)
    set(XXNETLIB_XXFCLIB_TARGET xxfclib::die_engine CACHE STRING "" FORCE)
    set(XXNETLIB_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(XXNETLIB_BUILD_CLI OFF CACHE BOOL "" FORCE)
    set(XXNETLIB_INSTALL OFF CACHE BOOL "" FORCE)
    set(XXNETLIB_ONLINE_TESTS OFF CACHE BOOL "" FORCE)
    add_subdirectory("${_die_mylibs}/xxnetlib" "${CMAKE_CURRENT_BINARY_DIR}/xxnetlib" EXCLUDE_FROM_ALL)
    set(XXGITHUB_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(XXGITHUB_BUILD_CLI OFF CACHE BOOL "" FORCE)
    set(XXGITHUB_INSTALL OFF CACHE BOOL "" FORCE)
    set(XXGITHUB_ONLINE_TESTS OFF CACHE BOOL "" FORCE)
    add_subdirectory("${_die_mylibs}/xxgithub" "${CMAKE_CURRENT_BINARY_DIR}/xxgithub" EXCLUDE_FROM_ALL)
endif()

# Native/portable NTInfo/die settings, using the same settings implementation
# as cdie's core.  The target is local to this build and does not involve Qt.
set(_die_xxfc_src "${_die_mylibs}/xxfclib/src")
add_library(die_update_settings STATIC
    "${_die_xxfc_src}/settings/xx_settings.c"
    "${_die_xxfc_src}/settings/xx_settings_ini.c"
    "${_die_xxfc_src}/settings/xx_settings_value.c"
    "${_die_xxfc_src}/global/xx_settings_global.c")
if(WIN32)
    target_sources(die_update_settings PRIVATE
        "${_die_xxfc_src}/settings/platforms/xx_settings_file_windows.c"
        "${_die_xxfc_src}/settings/platforms/xx_settings_windows.c")
else()
    target_sources(die_update_settings PRIVATE
        "${_die_xxfc_src}/settings/platforms/xx_settings_file_posix.c")
    target_compile_definitions(die_update_settings PRIVATE _POSIX_C_SOURCE=200809L)
    if(APPLE)
        target_sources(die_update_settings PRIVATE
            "${_die_xxfc_src}/settings/platforms/xx_settings_macos.c")
        target_compile_definitions(die_update_settings PRIVATE _DARWIN_C_SOURCE)
        target_link_libraries(die_update_settings PRIVATE "-framework CoreFoundation")
    else()
        target_sources(die_update_settings PRIVATE
            "${_die_xxfc_src}/settings/platforms/xx_settings_posix.c")
    endif()
endif()
target_link_libraries(die_update_settings PUBLIC xxfclib::die_engine)
set_property(TARGET die_update_settings PROPERTY C_STANDARD 99)

add_library(die_update_core STATIC
    "${DIE_CDIE_SOURCE_DIR}/src/updater/cdie_updater.c"
    "${DIE_CDIE_SOURCE_DIR}/src/updater/cdie_updater.h"
    "${DIE_CDIE_SOURCE_DIR}/src/app/cdie_app.c"
    "${DIE_CDIE_SOURCE_DIR}/src/app/cdie_app.h"
    "${DIE_CDIE_SOURCE_DIR}/src/app/cdie_paths.c"
    "${DIE_CDIE_SOURCE_DIR}/src/app/cdie_paths.h")
target_include_directories(die_update_core PUBLIC
    "${DIE_CDIE_SOURCE_DIR}/src/updater" "${DIE_CDIE_SOURCE_DIR}/src")
target_link_libraries(die_update_core PUBLIC xxfclib::die_engine die_update_settings PRIVATE xxgithub::xxgithub)
target_compile_definitions(die_update_core PUBLIC CDIE_HAVE_UPDATER=1 PRIVATE
    CDIE_USER_DATA_APPLICATION="die" X_APPLICATIONVERSION="${X_PROJECT_VERSION}")
set_property(TARGET die_update_core PROPERTY C_STANDARD 99)

add_executable(die_update
    "${DIE_CDIE_SOURCE_DIR}/src/core/utils_entry.c"
    "${DIE_CDIE_SOURCE_DIR}/src/updater/main_updater.c")
set_target_properties(die_update PROPERTIES OUTPUT_NAME "die-update")
target_include_directories(die_update PRIVATE "${DIE_CDIE_SOURCE_DIR}/src")
target_link_libraries(die_update PRIVATE die_update_core)
target_compile_definitions(die_update PRIVATE
    X_APPLICATIONNAME="die-update" X_APPLICATIONVERSION="${X_PROJECT_VERSION}")
set_property(TARGET die_update PROPERTY C_STANDARD 99)

if(UNIX AND NOT APPLE)
    target_compile_definitions(die_update_core PRIVATE CDIE_SYSTEM_INSTALL=1)
endif()
if(MSVC)
    target_compile_options(die_updater_qt PRIVATE /utf-8)
    target_compile_options(die_update_settings PRIVATE /utf-8)
    target_compile_options(die_update_core PRIVATE /utf-8)
    target_compile_options(die_update PRIVATE /utf-8)
endif()

if(UNIX AND NOT APPLE)
    include(GNUInstallDirs)
    install(TARGETS die_update RUNTIME DESTINATION "${CMAKE_INSTALL_LIBDIR}/die")
    install(CODE "
        file(MAKE_DIRECTORY \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}\")
        file(CREATE_LINK \"../${CMAKE_INSTALL_LIBDIR}/die/die-update\"
            \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/die-update\" SYMBOLIC)")
elseif(NOT APPLE)
    install(TARGETS die_update RUNTIME DESTINATION ".")
endif()

target_compile_definitions(die_updater_qt PUBLIC DIE_HAVE_UPDATER=1)
