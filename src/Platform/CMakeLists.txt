#
# Cobalt Platform library
#

set( COBALT_PLATFORM_SOURCES

)

set( COBALT_PLATFORM_HEADERS

)

# TODO Target-specific platform files

# Desktop
# Emscripten


source_group( Platform_cpp FILES ${COBALT_PLATFORM_SOURCES} ) 
source_group( Platform_hpp FILES ${COBALT_PLATFORM_HEADERS} ) 

add_library( cobaltPlatform STATIC ${COBALT_PLATFORM_SOURCES} ${COBALT_PLATFORM_HEADERS} )
