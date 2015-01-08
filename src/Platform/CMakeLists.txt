#
# Cobalt Platform library
#

set( COBALT_PLATFORM_SOURCES
    Application.cpp
)

set( COBALT_PLATFORM_HEADERS
    ../../include/Platform/Application.hpp
)

# TODO Target-specific platform files

#if( APPLE OR WINDOWS OR LINUX )
#    set( COBALT_PLATFORM_SOURCES ${COBALT_PLATFORM_SOURCES} Application_Desktop.cpp )
#endif()

# TODO Emscripten ?


source_group( Platform_cpp FILES ${COBALT_PLATFORM_SOURCES} ) 
source_group( Platform_hpp FILES ${COBALT_PLATFORM_HEADERS} ) 

add_library( cobalt_platform STATIC ${COBALT_PLATFORM_SOURCES} ${COBALT_PLATFORM_HEADERS} )
