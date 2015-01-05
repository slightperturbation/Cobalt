#
# Cobalt Core
#

set( COBALT_CORE_SOURCES

)

set( COBALT_CORE_HEADERS

)

source_group( Core_cpp FILES ${COBALT_CORE_SOURCES} ) 
source_group( Core_hpp FILES ${COBALT_CORE_HEADERS} ) 

add_library( cobaltCore STATIC ${COBALT_CORE_SOURCES} ${COBALT_CORE_HEADERS} )
