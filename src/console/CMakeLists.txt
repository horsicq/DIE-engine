set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(WIN32)
    add_definitions(-DNOMINMAX)
endif()

if(MSVC)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS _SCL_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_DEPRECATE)
    add_compile_options(/FS)
    add_compile_options($<$<CONFIG:Debug>:/Z7> $<$<CONFIG:RelWithDebInfo>:/Z7>)
endif()

add_definitions(-DUSE_DEX)
add_definitions(-DUSE_PDF)
add_definitions(-DUSE_ARCHIVE)
add_definitions(-DUSE_XSIMD)

include(${CMAKE_CURRENT_LIST_DIR}/../../die_script/die_script.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../XOptions/xoptions.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../XEntropyWidget/entropyprocess.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../XFileInfo/xfileinfo.cmake)

if(UNIX AND NOT APPLE)
    include(GNUInstallDirs)
endif()

set(DIEC_SOURCES
    ${DIE_SCRIPT_SOURCES}
    ${XOPTIONS_SOURCES}
    ${ENTROPYPROCESS_SOURCES}
    ${XFILEINFO_SOURCES}
    main_console.cpp
    consoleoutput.cpp
    consoleoutput.h
)

if(WIN32 AND EXISTS "${X_WINDOWS_RESOURCE_RC_IN}")
    set(X_ORIGINAL_FILENAME "diec")
    configure_file("${X_WINDOWS_RESOURCE_RC_IN}"
                   "${CMAKE_CURRENT_BINARY_DIR}/resource_diec.rc" @ONLY)
    configure_file("${X_RESOURCE_DIR}/windows/windows.manifest.xml"
                   "${CMAKE_CURRENT_BINARY_DIR}/windows.manifest.xml" COPYONLY)
    list(APPEND DIEC_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/resource_diec.rc")
endif()

add_executable(diec ${DIEC_SOURCES})

if(MSVC)
    target_link_options(diec PRIVATE /MANIFEST:NO)
endif()

target_include_directories(diec PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(diec PRIVATE
    xsimd
    bzip2
    lzma
    zlib
    capstone_x86
    ppmd
    Qt${DIE_QT_MAJOR}::Core
    Qt${DIE_QT_MAJOR}::Concurrent
)

if(DIE_QT_MAJOR EQUAL 5)
    target_link_libraries(diec PRIVATE Qt5::Script)
elseif(DIE_QT_MAJOR GREATER_EQUAL 6)
    target_link_libraries(diec PRIVATE Qt${DIE_QT_MAJOR}::Qml)
endif()

if(WIN32)
    target_link_libraries(diec PRIVATE Wintrust Crypt32)
endif()

if(TARGET die_translations)
    add_dependencies(diec die_translations)
endif()

if(UNIX AND NOT APPLE)
    install(TARGETS diec RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
else()
    install(TARGETS diec RUNTIME DESTINATION ".")
endif()
