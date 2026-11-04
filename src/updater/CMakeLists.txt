# The Qt frontends and updater share the same application path resolution.
add_library(die_updater_qt STATIC dieupdater.cpp dieupdater.h)
target_include_directories(die_updater_qt PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(die_updater_qt PUBLIC Qt${DIE_QT_MAJOR}::Core)
target_compile_features(die_updater_qt PUBLIC cxx_std_11)

option(DIE_BUILD_UPDATER "Build die-update and database-update launchers" ON)
if(NOT DIE_BUILD_UPDATER)
    return()
endif()

# Reuse the ZIP codec already shipped with the Qt XArchive dependency. This
# updater links only the codec and Qt Core/Network.
set(_die_zip_algos "${CMAKE_SOURCE_DIR}/dep/XArchive/Algos")
add_library(die_update_core STATIC
    databaseupdater.cpp databaseupdater.h
    databasearchive.cpp databasearchive.h
    "${_die_zip_algos}/zlibdeclib.cpp"
    "${_die_zip_algos}/zlibutil.cpp")
target_include_directories(die_update_core PRIVATE
    "${_die_zip_algos}" "${_die_zip_algos}/include")
target_link_libraries(die_update_core PUBLIC die_updater_qt PRIVATE Qt${DIE_QT_MAJOR}::Network)
target_compile_definitions(die_update_core PRIVATE DIE_UPDATE_VERSION="${X_PROJECT_VERSION}")

add_executable(die_update main_updater.cpp)
set_target_properties(die_update PROPERTIES OUTPUT_NAME "die-update")
target_link_libraries(die_update PRIVATE die_update_core)
target_compile_definitions(die_update PRIVATE DIE_UPDATE_VERSION="${X_PROJECT_VERSION}")

if(MSVC)
    target_compile_options(die_updater_qt PRIVATE /utf-8)
    target_compile_options(die_update_core PRIVATE /utf-8)
    target_compile_options(die_update PRIVATE /utf-8)
endif()

if(UNIX AND NOT APPLE)
    # Package-managed installs keep the shipped database untouched. Portable
    # markers still opt out of the per-user fallback in the updater itself.
    target_compile_definitions(die_update_core PRIVATE DIE_SYSTEM_INSTALL=1)
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
