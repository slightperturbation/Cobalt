#
# Root CMake file for Cobalt
# See BUILD.md for guidance on building
#
get_filename_component( COBALT_ROOT_DIR "../Cobalt" ABSOLUTE )
get_filename_component( COBALT_PROJECT_DIR "../Cobalt" ABSOLUTE )

project( cobalt )

# Create common macros
include( "cmake/cobalt.cmake" )

# Define configurable options
include( "cmake/cobalt_options.cmake")

# Define macros for system dependencies 
include( "cmake/cobalt_dependencies.cmake" )

# Create platform-specific macros 
if( APPLE )
    include( "cmake/cobalt_osx.cmake" )
elseif( WINDOWS )
    include( "cmake/cobalt_windows.cmake" )
else()
    message( FATAL_ERROR "Platform not currently supported." )
endif()

# Apply macros defined above to setup dependencies
cobalt_compiler_settings()
cobalt_include_glm()
cobalt_include_glew()
cobalt_include_glfw()
cobalt_include_opengl()

#cobalt_set_bin_output_directory() --- TODO
cobalt_add_shaders_to_project( ${COBALT_SHADER_DIRECTORY} )

# Set header search path for Cobalt itself
include_directories( include )

# Set header search paths for included header-only dependencies
#include_directories(  )

add_subdirectory( src/Core )
add_subdirectory( src/Graphics )
add_subdirectory( src/Platform )

if( COBALT_BUILD_EXAMPLES )
    add_subdirectory( examples )
endif()
