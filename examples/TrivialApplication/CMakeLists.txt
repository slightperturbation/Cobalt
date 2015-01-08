#
# TrivialApplication example, demostrates the runtime loop
#

set( COBALT_TRIVIALAPPLICATION_SOURCES
    TrivialApplication.cpp
)

set( COBALT_TRIVIALAPPLICATION_HEADERS

)

source_group( examples/HelloWorld_cpp ${COBALT_TRIVIALAPPLICATION_SOURCES} )
source_group( examples/HelloWorld_hpp ${COBALT_TRIVIALAPPLICATION_HEADERS} )

# Add shaders to the project so they can be edited using an IDE
#cobalt_add_shaders_to_project( "./shaders" )

add_executable( cobalt_trivial_application ${COBALT_TRIVIALAPPLICATION_SOURCES} ${COBALT_TRIVIALAPPLICATION_HEADERS} )

# Link with cobalt libraries
target_link_libraries( cobalt_trivial_application
    cobalt_graphics cobalt_platform cobalt_core )

# Link with cobalt's external libraries
cobalt_link_external_libraries( cobalt_trivial_application )
