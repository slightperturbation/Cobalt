#
# Cobalt Graphics library
#

set( COBALT_GRAPHICS_SOURCES
    Shader.cpp
)

set( COBALT_GRAPHICS_HEADERS
    ../../include/Graphics/Shader.hpp
)

source_group( Graphics_cpp FILES ${COBALT_GRAPHICS_SOURCES} ) 
source_group( Graphics_hpp FILES ${COBALT_GRAPHICS_HEADERS} ) 

add_library( cobalt_graphics STATIC ${COBALT_GRAPHICS_SOURCES} ${COBALT_GRAPHICS_HEADERS} )
