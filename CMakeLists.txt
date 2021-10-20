cmake_minimum_required(VERSION 3.5)

project(MainProject)

if(WIN32)
    set (CMAKE_PREFIX_PATH "C:/Qt/5.15.2/msvc2019/")
endif()

if(APPLE)
    set (CMAKE_PREFIX_PATH $ENV{HOME}/Qt/5.15.2/clang_64/)
    set (CMAKE_OSX_ARCHITECTURES x86_64)
    add_compile_options(-Wno-deprecated-declarations)
    add_compile_options(-Wno-switch)
endif()

add_subdirectory(XCppfilt)
add_subdirectory(XCapstone)
add_subdirectory(XArchive)
add_subdirectory(gui_source)