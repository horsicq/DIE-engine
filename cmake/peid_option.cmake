option(WITH_PEID "Use PEiD scan engine" OFF)

if(WITH_PEID)
    add_compile_definitions(USE_PEID)
endif()
