# Qt integration and the unchanged cdie signature-database updater.

add_library(die_updater_qt STATIC dieupdater.cpp dieupdater.h)
target_include_directories(die_updater_qt PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(die_updater_qt PUBLIC Qt${DIE_QT_MAJOR}::Core)

option(DIE_BUILD_UPDATER "Build die-update and database-update launchers" ON)
set(DIE_CDIE_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../cdie_source" CACHE PATH
    "cdie source tree that supplies the shared die-update implementation")

if(NOT DIE_BUILD_UPDATER)
    return()
endif()

set(_die_cdie_src "${DIE_CDIE_SOURCE_DIR}/src")
set(_die_xxfclib_dir "${CMAKE_SOURCE_DIR}/dep/xxfclib")
set(_die_xxnetlib_dir "${CMAKE_SOURCE_DIR}/dep/xxnetlib")
set(_die_xxgithub_dir "${CMAKE_SOURCE_DIR}/dep/xxgithub")

foreach(_die_required
        "${_die_cdie_src}/updater/cdie_updater.c"
        "${_die_cdie_src}/updater/main_updater.c"
        "${_die_xxfclib_dir}/CMakeLists.txt"
        "${_die_xxnetlib_dir}/CMakeLists.txt"
        "${_die_xxgithub_dir}/CMakeLists.txt")
    if(NOT EXISTS "${_die_required}")
        # Trees without cdie_source / xxnetlib / xxgithub (e.g. a GitHub
        # checkout of die_source) still build; only die-update is left out.
        message(WARNING "die-update is not built: ${_die_required} is missing. Set DIE_CDIE_SOURCE_DIR when cdie_source is elsewhere.")
        return()
    endif()
endforeach()

set(XXFC_DIE_ENGINE_TAR_ONLY_WRITING ON)
set(XXFC_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(XXFC_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(XXFC_BUILD_DIE_ENGINE ON CACHE BOOL "" FORCE)
set(XXFC_CODEPAGE_JAPANESE OFF CACHE BOOL "" FORCE)
set(XXFC_CODEPAGE_CHINESE OFF CACHE BOOL "" FORCE)
set(XXFC_CODEPAGE_KOREAN OFF CACHE BOOL "" FORCE)
set(XXFC_ENABLE_STREAM_DATASTRUCTS ON)
set(XXFC_ENABLE_STREAM_IMPORT ON)
set(XXFC_ENABLE_STREAM_EXPORT ON)
set(XXFC_ENABLE_STREAM_RESOURCES ON)
set(XXFC_ENABLE_STREAM_METADATA OFF)
set(XXFC_ENABLE_STREAM_SYMBOLS ON)

if(NOT TARGET xxfclib::die_engine)
    add_subdirectory("${_die_xxfclib_dir}" "${CMAKE_BINARY_DIR}/xxfclib" EXCLUDE_FROM_ALL)
endif()
if(COMMAND xxfclib_require_streams AND TARGET xxfclib::die_engine)
    xxfclib_require_streams(xxfclib::die_engine DATASTRUCTS)
endif()

set(XXNETLIB_XXFCLIB_TARGET xxfclib::die_engine CACHE STRING "" FORCE)
set(XXNETLIB_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(XXNETLIB_BUILD_TESTKIT OFF CACHE BOOL "" FORCE)
set(XXNETLIB_BUILD_CLI OFF CACHE BOOL "" FORCE)
set(XXNETLIB_INSTALL OFF CACHE BOOL "" FORCE)
set(XXNETLIB_ONLINE_TESTS OFF CACHE BOOL "" FORCE)
if(NOT TARGET xxnetlib::xxnetlib)
    add_subdirectory("${_die_xxnetlib_dir}" "${CMAKE_BINARY_DIR}/xxnetlib" EXCLUDE_FROM_ALL)
endif()

set(XXGITHUB_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(XXGITHUB_BUILD_CLI OFF CACHE BOOL "" FORCE)
set(XXGITHUB_INSTALL OFF CACHE BOOL "" FORCE)
set(XXGITHUB_ONLINE_TESTS OFF CACHE BOOL "" FORCE)
if(NOT TARGET xxgithub::xxgithub)
    add_subdirectory("${_die_xxgithub_dir}" "${CMAKE_BINARY_DIR}/xxgithub" EXCLUDE_FROM_ALL)
endif()

set(_die_settings_src "${_die_xxfclib_dir}/src")
add_library(die_update_settings STATIC
    "${_die_settings_src}/settings/xx_settings.c"
    "${_die_settings_src}/settings/xx_settings_ini.c"
    "${_die_settings_src}/settings/xx_settings_value.c"
    "${_die_settings_src}/global/xx_settings_global.c")
if(WIN32)
    target_sources(die_update_settings PRIVATE
        "${_die_settings_src}/settings/platforms/xx_settings_file_windows.c"
        "${_die_settings_src}/settings/platforms/xx_settings_windows.c")
else()
    target_sources(die_update_settings PRIVATE "${_die_settings_src}/settings/platforms/xx_settings_file_posix.c")
    target_compile_definitions(die_update_settings PRIVATE _POSIX_C_SOURCE=200809L)
    if(APPLE)
        target_sources(die_update_settings PRIVATE "${_die_settings_src}/settings/platforms/xx_settings_macos.c")
        target_compile_definitions(die_update_settings PRIVATE _DARWIN_C_SOURCE)
        target_link_libraries(die_update_settings PRIVATE "-framework CoreFoundation")
    else()
        target_sources(die_update_settings PRIVATE "${_die_settings_src}/settings/platforms/xx_settings_posix.c")
    endif()
endif()
target_link_libraries(die_update_settings PUBLIC xxfclib::die_engine)

add_library(die_update_core STATIC
    "${_die_cdie_src}/updater/cdie_updater.c"
    "${_die_cdie_src}/updater/cdie_updater.h"
    "${_die_cdie_src}/app/cdie_app.c"
    "${_die_cdie_src}/app/cdie_app.h"
    "${_die_cdie_src}/app/cdie_paths.c"
    "${_die_cdie_src}/app/cdie_paths.h")
target_include_directories(die_update_core PUBLIC "${_die_cdie_src}/updater" "${_die_cdie_src}")
target_link_libraries(die_update_core PUBLIC xxfclib::die_engine die_update_settings PRIVATE xxgithub::xxgithub)
target_compile_definitions(die_update_core PUBLIC DIE_HAVE_UPDATER=1)
if(UNIX AND NOT APPLE)
    target_compile_definitions(die_update_core PRIVATE CDIE_SYSTEM_INSTALL=1)
endif()

add_executable(die_update
    "${_die_cdie_src}/core/utils_entry.c"
    "${_die_cdie_src}/updater/main_updater.c")
set_target_properties(die_update PROPERTIES OUTPUT_NAME "die-update")
target_compile_definitions(die_update PRIVATE X_APPLICATIONNAME="die-update")
target_include_directories(die_update PRIVATE "${_die_cdie_src}")
target_link_libraries(die_update PRIVATE die_update_core)

if(MSVC)
    target_compile_options(die_update_core PRIVATE /utf-8)
    target_compile_options(die_update PRIVATE /utf-8)
endif()

if(UNIX AND NOT APPLE)
    include(GNUInstallDirs)
    install(TARGETS die_update RUNTIME DESTINATION "${CMAKE_INSTALL_LIBDIR}/die")
    install(CODE
        "file(MAKE_DIRECTORY \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}\")\n"
        "file(CREATE_LINK \"../${CMAKE_INSTALL_LIBDIR}/die/die-update\" \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/die-update\" SYMBOLIC)")
elseif(NOT APPLE)
    install(TARGETS die_update RUNTIME DESTINATION ".")
endif()

target_compile_definitions(die_updater_qt PUBLIC DIE_HAVE_UPDATER=1)
