#
# HelloWorld example 
#

set( COBALT_HELLOWORLD_SOURCES 
    HelloWorld.cpp
)

set( COBALT_HELLOWORLD_HEADERS

)

source_group( examples/HelloWorld_cpp ${COBALT_HELLOWORLD_SOURCES} )source_group( examples/HelloWorld_hpp ${COBALT_HELLOWORLD_HEADERS} )

add_executable( cobalt_helloworld ${COBALT_HELLOWORLD_SOURCES} ${COBALT_HELLOWORLD_HEADERS} ) 

target_link_libraries( cobalt_helloworld cobalt_graphics )
target_link_libraries( cobalt_helloworld cobalt_platform )target_link_libraries( cobalt_helloworld cobalt_core )

cobalt_link_external_libraries( cobalt_helloworld )
