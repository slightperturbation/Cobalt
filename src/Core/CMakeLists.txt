#
# Cobalt Core
#

set( COBALT_CORE_SOURCES
    Log.cpp
)

set( COBALT_CORE_HEADERS
    ../../include/Core/Log.hpp
)

source_group( Core_cpp FILES ${COBALT_CORE_SOURCES} ) 
source_group( Core_hpp FILES ${COBALT_CORE_HEADERS} ) 

add_library( cobalt_core STATIC ${COBALT_CORE_SOURCES} ${COBALT_CORE_HEADERS} )
