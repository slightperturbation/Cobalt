#
# Define macros for including dependencies that are expected to be part of the system
# (i.e., not bundlded in Cobalt)
#

macro( cobalt_include_glm )
    add_definitions( -DGLM_FORCE_RADIANS )

    # GLM -- on Windows: depends on env var: GLM_ROOT_DIR
    find_package(glm REQUIRED)
    if( GLM_FOUND )
        include_directories( ${GLM_INCLUDE_DIR} )
    else()
        if( WINDOWS )
            message( FATAL_ERROR "Failed to find GLM; check the GLM_ROOT_DIR environment variable.")
        else()
            message( FATAL_ERROR "Failed to find GLM")
        endif()
    endif()
endmacro()

macro( cobalt_include_glew )
    find_package(glew REQUIRED)
    if( GLEW_FOUND )
      include_directories (${GLEW_INCLUDE_DIR})
      if( WIN32 )
        list( APPEND COBALT_GLEW_LIBRARIES debug ${GLEW_DEBUG_LIBRARY} )
        list( APPEND COBALT_GLEW_LIBRARIES optimized ${GLEW_RELEASE_LIBRARY} )
      else( WIN32 )
        list( APPEND COBALT_GLEW_LIBRARIES ${GLEW_LIBRARY} )
      endif( WIN32 )
    else( GLEW_FOUND )
      message( FATAL_ERROR "GLEW NOT FOUND")
    endif( GLEW_FOUND )
endmacro()

macro( cobalt_include_glfw )
  # TODO -- what about windows?  should GLFW be included in ext?
  find_package( PkgConfig REQUIRED )
  pkg_search_module( GLFW REQUIRED glfw3 )
  include_directories( ${GLFW_INCLUDE_DIRS} )
  set( COBALT_GLFW_LIBRARIES ${GLFW_LIBRARIES} )

  # Ask for OpenGL 3 headers
  add_definitions( -DGLFW_INCLUDE_GLCOREARB )

  # Separate flags by spaces, as expected by set_target_properties, instead of semi-colon separated
  string( REPLACE ";" " " COBALT_GLFW_EXPANDED_CFLAGS "${GLFW_CFLAGS}")
  string( REPLACE ";" " " COBALT_GLFW_EXPANDED_LDFLAGS "${GLFW_LDFLAGS}")
  # NOTE -- executables must add these EXPANDED flags in set_target_properties
endmacro()

macro( cobalt_include_opengl )
  find_package( OpenGL REQUIRED )
  include_directories( ${OPENGL_INCLUDE_DIR} )
  set( COBALT_OPENGL_LIBRARIES ${LIBS} ${OPENGL_LIBRARIES} )
  if( APPLE )
      find_library( COCOA_LIBRARY Cocoa )
      find_library( IOKIT_LIBRARY IOKit )
      find_library( COREVIDEO_LIBRARY CoreVideo )
      set( COBALT_OPENGL_LIBRARIES ${COBALT_OPENGL_LIBRARIES} ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY} )
  endif()
endmacro()

# Each executable must be linked to the external libraries
macro( cobalt_link_external_libraries EXECUTABLE_NAME )
  set_target_properties( ${EXECUTABLE_NAME} 
      PROPERTIES
      COMPILE_FLAGS "${COBALT_GLFW_EXPANDED_CFLAGS}"  # Space separated
      LINK_FLAGS "${COBALT_GLFW_EXPANDED_LDFLAGS}"  # Space separated
  )
  target_link_libraries( ${EXECUTABLE_NAME} ${COBALT_GLEW_LIBRARIES} )
  target_link_libraries( ${EXECUTABLE_NAME} ${COBALT_OPENGL_LIBRARIES} )
  target_link_libraries( ${EXECUTABLE_NAME} ${COBALT_GLFW_LIBRARIES} )
endmacro()

