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

# Utility rule file for install-LLVMTarget.

# Include the progress variables for this target.
include lib/Target/CMakeFiles/install-LLVMTarget.dir/progress.make

lib/Target/CMakeFiles/install-LLVMTarget: lib/libLLVMTarget.a
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/lib/Target && /usr/local/Cellar/cmake/3.3.1/bin/cmake -DCMAKE_INSTALL_COMPONENT=LLVMTarget -P /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/cmake_install.cmake

install-LLVMTarget: lib/Target/CMakeFiles/install-LLVMTarget
install-LLVMTarget: lib/Target/CMakeFiles/install-LLVMTarget.dir/build.make

.PHONY : install-LLVMTarget

# Rule to build all files generated by this target.
lib/Target/CMakeFiles/install-LLVMTarget.dir/build: install-LLVMTarget

.PHONY : lib/Target/CMakeFiles/install-LLVMTarget.dir/build

lib/Target/CMakeFiles/install-LLVMTarget.dir/clean:
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/lib/Target && $(CMAKE_COMMAND) -P CMakeFiles/install-LLVMTarget.dir/cmake_clean.cmake
.PHONY : lib/Target/CMakeFiles/install-LLVMTarget.dir/clean

lib/Target/CMakeFiles/install-LLVMTarget.dir/depend:
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/lib/Target /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64 /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/lib/Target /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/lib/Target/CMakeFiles/install-LLVMTarget.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/Target/CMakeFiles/install-LLVMTarget.dir/depend

