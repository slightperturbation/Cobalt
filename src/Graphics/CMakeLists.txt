#
# Cobalt Graphics library
#

set( COBALT_GRAPHICS_SOURCES

)

set( COBALT_GRAPHICS_HEADERS

)

source_group( Graphics_cpp FILES ${COBALT_GRAPHICS_SOURCES} ) 
source_group( Graphics_hpp FILES ${COBALT_GRAPHICS_HEADERS} ) 

add_library( cobaltGraphics STATIC ${COBALT_GRAPHICS_SOURCES} ${COBALT_GRAPHICS_HEADERS} )
