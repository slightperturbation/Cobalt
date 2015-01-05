#
# Configuration specific to Windows targets
# 


# Prefer static libraries
set( CMAKE_FIND_LIBRARY_SUFFIXES .lib .a ${CMAKE_FIND_LIBRARY_SUFFIXES} )

# Bug in some versions of visual studio creates duplicate manifest entries
# Avoiding manifests entirely seems to be one fix. (http://www.cmake.org/Bug/print_bug_page.php?bug_id=8386)
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /MANIFEST:NO")
if( MSVC )
    # warning C4530: C++ exception handler used, but unwind semantics are not 
    # enabled. Specify /EHsc 
    # MSVC needs /EHsc flag, which I included in root CMakeLists.txt, but 
    # above SET(CMAKE_CXX_FLAGS "") erases it.
    set( CMAKE_CXX_FLAGS "/EHsc" )
endif( MSVC )