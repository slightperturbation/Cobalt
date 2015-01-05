#
# Cobalt-wide settings and configuration
# 

cmake_minimum_required( VERSION 2.8 FATAL_ERROR )
enable_language(C)
enable_language(CXX)

set( CMAKE_MODULE_PATH "./cmake" "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH} )

macro( cobalt_use_modern_cpp )

    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("-std=c++1y" COMPILER_SUPPORTS_CXX1Y)
    if(COMPILER_SUPPORTS_CXX1Y)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
    else(COMPILER_SUPPORTS_CXX1Y)
        CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
        if(COMPILER_SUPPORTS_CXX11)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
        else(COMPILER_SUPPORTS_CXX11)
            CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
            if(COMPILER_SUPPORTS_CXX0X)
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
            else(COMPILER_SUPPORTS_CXX0X)
                message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Use clang.")
            endif(COMPILER_SUPPORTS_CXX0X)
        endif(COMPILER_SUPPORTS_CXX11)
    endif(COMPILER_SUPPORTS_CXX1Y)
    if( APPLE )
        set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++1y" )
        set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
    endif( APPLE )

endmacro()

macro( cobalt_compiler_settings )

    cobalt_use_modern_cpp()

    if( COBALT_NO_THREADS )
        add_definitions( -DCOBALT_NO_THREADS=1 )
    else()
        add_definitions( -DCOBALT_NO_THREADS=0 )
    endif()

endmacro()

macro( cobalt_add_shaders_to_project SHADER_DIR )

    file(GLOB VERT_SHADER_FILES RELATIVE 
      ${SHADER_DIR} "${SHADER_DIR}/*.vert" )
    file(GLOB FRAG_SHADER_FILES RELATIVE 
      ${SHADER_DIR} "${SHADER_DIR}/*.frag" )

    foreach( shaderFile ${VERT_SHADER_FILES} )
      LIST(APPEND VERT_SHADERS "${SHADER_DIR}/${shaderFile}" )
      SET_SOURCE_FILES_PROPERTIES( ${SHADER_DIR}/${shaderFile} PROPERTIES HEADER_FILE_ONLY TRUE )
    endforeach()
    foreach( shaderFile ${FRAG_SHADER_FILES} )
      LIST(APPEND FRAG_SHADERS "${SHADER_DIR}/${shaderFile}" )
      SET_SOURCE_FILES_PROPERTIES( ${SHADER_DIR}/${shaderFile} PROPERTIES HEADER_FILE_ONLY TRUE )
    endforeach()

endmacro()


macro( cobalt_set_bin_output_directory )

  set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
  set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
  set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

endmacro()



