# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.3

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.3.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.3.1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64

# Utility rule file for install-llvm-profdata.

# Include the progress variables for this target.
include tools/llvm-profdata/CMakeFiles/install-llvm-profdata.dir/progress.make

tools/llvm-profdata/CMakeFiles/install-llvm-profdata: bin/llvm-profdata
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/llvm-profdata && /usr/local/Cellar/cmake/3.3.1/bin/cmake -DCMAKE_INSTALL_COMPONENT=llvm-profdata -P /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/cmake_install.cmake

install-llvm-profdata: tools/llvm-profdata/CMakeFiles/install-llvm-profdata
install-llvm-profdata: tools/llvm-profdata/CMakeFiles/install-llvm-profdata.dir/build.make

.PHONY : install-llvm-profdata

# Rule to build all files generated by this target.
tools/llvm-profdata/CMakeFiles/install-llvm-profdata.dir/build: install-llvm-profdata

.PHONY : tools/llvm-profdata/CMakeFiles/install-llvm-profdata.dir/build

tools/llvm-profdata/CMakeFiles/install-llvm-profdata.dir/clean:
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/llvm-profdata && $(CMAKE_COMMAND) -P CMakeFiles/install-llvm-profdata.dir/cmake_clean.cmake
.PHONY : tools/llvm-profdata/CMakeFiles/install-llvm-profdata.dir/clean

tools/llvm-profdata/CMakeFiles/install-llvm-profdata.dir/depend:
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/tools/llvm-profdata /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64 /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/llvm-profdata /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/llvm-profdata/CMakeFiles/install-llvm-profdata.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tools/llvm-profdata/CMakeFiles/install-llvm-profdata.dir/depend

