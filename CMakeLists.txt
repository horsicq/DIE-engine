cmake_minimum_required(VERSION 3.18)
project(DetectItEasy)

# CMake 3.31 normalizes install() DESTINATION paths under CMP0177. On Windows the
# runtime data root (X_RESOURCES) is ".", so the _mylibs data installs reach
# install() as "./db" and warn until the policy is set. Normalized destinations
# are what this project wants, so opt in rather than carry the warning.
if(POLICY CMP0177)
    cmake_policy(SET CMP0177 NEW)
endif()
add_subdirectory(src)
