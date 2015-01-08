
#
# Cobalt-wide OSX-specific settings 
#

# Prefer static libraries
set( CMAKE_FIND_LIBRARY_SUFFIXES .a ${CMAKE_FIND_LIBRARY_SUFFIXES} )

if( APPLE )
    # Cmake defaults to using X86 and x64, but we want to use 
    # XCode's defaults instead
    # set( CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD)" )
endif()
