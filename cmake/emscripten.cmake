#
# Cmake toolchain file for emscripten
#
# To use this toolchain, specify -DCMAKE_TOOLCHAIN_FILE=emscripten.cmake
#
# E.g.:
#
#  cmake .. -DCMAKE_TOOLCHAIN_FILE=/Users/vector/Projects/Cobalt/cmake/emscripten.cmake
#
set( CMAKE_SYSTEM_NAME Emscripten ) # Hide "System is unknown to cmake" warnings
set( CMAKE_CROSSCOMPILING TRUE )

set( COBALT_EMSCRIPTEN TRUE )

set( COBALT_EMSCRIPTEN_PATH "${CMAKE_SOURCE_DIR}/ext/emsdk_portable" CACHE FILESYSTEM_PATH "Path to the root of the emscripten directory (containing emsdk)" )

# TODO -- These are version specific and quite fragile! 
set( COBALT_EMSCRIPTEN_TOOLCHAIN_PATH "${COBALT_EMSCRIPTEN_PATH}/emscripten/1.27.0/")
set( COBALT_EMSCRIPTEN_NODE_PATH "${COBALT_EMSCRIPTEN_PATH}/node/0.10.18_64bit/bin/" )
set( COBALT_EMSCRIPTEN_CLANG_PATH "${COBALT_EMSCRIPTEN_PATH}/clang/e1.27.0_64bit/" )

set( ENV{EMSCRIPTEN} ${COBALT_EMSCRIPTEN_TOOLCHAIN_PATH} )

#include( CMakeForceCompiler )
#CMAKE_FORCE_C_COMPILER( "${CMAKE_C_COMPILER}" Clang)
#CMAKE_FORCE_CXX_COMPILER( "${CMAKE_CXX_COMPILER}" Clang)

# Cross compilers
set( CMAKE_C_COMPILER "${COBALT_EMSCRIPTEN_TOOLCHAIN_PATH}/emcc" )
set( CMAKE_CXX_COMPILER "${COBALT_EMSCRIPTEN_TOOLCHAIN_PATH}/em++" )
set( CMAKE_AR "${COBALT_EMSCRIPTEN_TOOLCHAIN_PATH}/emar" )
set( CMAKE_LINKER "${COBALT_EMSCRIPTEN_TOOLCHAIN_PATH}/emcc" )
set( CMAKE_RANLIB "${COBALT_EMSCRIPTEN_TOOLCHAIN_PATH}/emranlib" )

set( CMAKE_FIND_ROOT_PATH ${CMAKE_FIND_ROOT_PATH} 
    ${COBALT_EMSCRIPTEN_TOOLCHAIN_PATH} 
    ${COBALT_EMSCRIPTEN_PATH} 
    ${COBALT_EMSCRIPTEN_NODE_PATH}
    ${COBALT_EMSCRIPTEN_CLANG_PATH}
)

# only search for libraries and includes in the toolchain
set( CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER )
set( CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY )
set( CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH )
set( CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY )

set( CMAKE_CONFIGURATION_TYPES Debug Release )
set( CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG" )
set( CMAKE_CXX_FLAGS_DEBUG "-O3 -g -D_DEBUG_ -D_DEBUG" )

