#
# HelloWorld example 
#

set( COBALT_HELLOWORLD_SOURCES 
    HelloWorld.cpp
)

set( COBALT_HELLOWORLD_HEADERS

)

source_group( examples/HelloWorld_cpp ${COBALT_HELLOWORLD_SOURCES} )
source_group( examples/HelloWorld_hpp ${COBALT_HELLOWORLD_HEADERS} )

# Add shaders to the project so they can be edited using an IDE
#cobalt_add_shaders_to_project( "./shaders" )

add_executable( cobalt_helloworld ${COBALT_HELLOWORLD_SOURCES} ${COBALT_HELLOWORLD_HEADERS} )

# Link with cobalt libraries
target_link_libraries( cobalt_helloworld
    cobalt_graphics cobalt_platform cobalt_core )

# Link with cobalt's external libraries
cobalt_link_external_libraries( cobalt_helloworld )
