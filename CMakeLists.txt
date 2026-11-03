cmake_minimum_required(VERSION 3.18)
project(DetectItEasy)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/peid_option.cmake)

# CMake 3.31 normalizes install() DESTINATION paths under CMP0177. On Windows the
# runtime data root (X_RESOURCES) is ".", so the _mylibs data installs reach
# install() as "./db" and warn until the policy is set. Normalized destinations
# are what this project wants, so opt in rather than carry the warning.
if(POLICY CMP0177)
    cmake_policy(SET CMP0177 NEW)
endif()
option(DIE_UPDATER_ONLY "Build just the Qt database updater" OFF)
if(DIE_UPDATER_ONLY)
    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network)
    set(DIE_QT_MAJOR ${QT_VERSION_MAJOR})
    find_package(Qt${DIE_QT_MAJOR} REQUIRED COMPONENTS Core Network)
    file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/release_version.txt" X_PROJECT_VERSION)
    add_subdirectory(src/updater)
else()
    add_subdirectory(src)
endif()
