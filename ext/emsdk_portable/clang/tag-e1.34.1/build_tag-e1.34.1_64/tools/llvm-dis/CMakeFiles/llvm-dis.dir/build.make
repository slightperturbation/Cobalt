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

# Include any dependencies generated for this target.
include tools/llvm-dis/CMakeFiles/llvm-dis.dir/depend.make

# Include the progress variables for this target.
include tools/llvm-dis/CMakeFiles/llvm-dis.dir/progress.make

# Include the compile flags for this target's objects.
include tools/llvm-dis/CMakeFiles/llvm-dis.dir/flags.make

tools/llvm-dis/CMakeFiles/llvm-dis.dir/llvm-dis.cpp.o: tools/llvm-dis/CMakeFiles/llvm-dis.dir/flags.make
tools/llvm-dis/CMakeFiles/llvm-dis.dir/llvm-dis.cpp.o: /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/tools/llvm-dis/llvm-dis.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tools/llvm-dis/CMakeFiles/llvm-dis.dir/llvm-dis.cpp.o"
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/llvm-dis && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/llvm-dis.dir/llvm-dis.cpp.o -c /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/tools/llvm-dis/llvm-dis.cpp

tools/llvm-dis/CMakeFiles/llvm-dis.dir/llvm-dis.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/llvm-dis.dir/llvm-dis.cpp.i"
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/llvm-dis && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/tools/llvm-dis/llvm-dis.cpp > CMakeFiles/llvm-dis.dir/llvm-dis.cpp.i

tools/llvm-dis/CMakeFiles/llvm-dis.dir/llvm-dis.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/llvm-dis.dir/llvm-dis.cpp.s"
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/llvm-dis && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/tools/llvm-dis/llvm-dis.cpp -o CMakeFiles/llvm-dis.dir/llvm-dis.cpp.s

tools/llvm-dis/CMakeFiles/llvm-dis.dir/llvm-dis.cpp.o.requires:

.PHONY : tools/llvm-dis/CMakeFiles/llvm-dis.dir/llvm-dis.cpp.o.requires

tools/llvm-dis/CMakeFiles/llvm-dis.dir/llvm-dis.cpp.o.provides: tools/llvm-dis/CMakeFiles/llvm-dis.dir/llvm-dis.cpp.o.requires
	$(MAKE) -f tools/llvm-dis/CMakeFiles/llvm-dis.dir/build.make tools/llvm-dis/CMakeFiles/llvm-dis.dir/llvm-dis.cpp.o.provides.build
.PHONY : tools/llvm-dis/CMakeFiles/llvm-dis.dir/llvm-dis.cpp.o.provides

tools/llvm-dis/CMakeFiles/llvm-dis.dir/llvm-dis.cpp.o.provides.build: tools/llvm-dis/CMakeFiles/llvm-dis.dir/llvm-dis.cpp.o


# Object files for target llvm-dis
llvm__dis_OBJECTS = \
"CMakeFiles/llvm-dis.dir/llvm-dis.cpp.o"

# External object files for target llvm-dis
llvm__dis_EXTERNAL_OBJECTS =

bin/llvm-dis: tools/llvm-dis/CMakeFiles/llvm-dis.dir/llvm-dis.cpp.o
bin/llvm-dis: tools/llvm-dis/CMakeFiles/llvm-dis.dir/build.make
bin/llvm-dis: lib/libLLVMBitReader.a
bin/llvm-dis: lib/libLLVMCore.a
bin/llvm-dis: lib/libLLVMNaClBitReader.a
bin/llvm-dis: lib/libLLVMSupport.a
bin/llvm-dis: lib/libLLVMNaClAnalysis.a
bin/llvm-dis: lib/libLLVMAnalysis.a
bin/llvm-dis: lib/libLLVMCore.a
bin/llvm-dis: lib/libLLVMSupport.a
bin/llvm-dis: tools/llvm-dis/CMakeFiles/llvm-dis.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../bin/llvm-dis"
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/llvm-dis && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/llvm-dis.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tools/llvm-dis/CMakeFiles/llvm-dis.dir/build: bin/llvm-dis

.PHONY : tools/llvm-dis/CMakeFiles/llvm-dis.dir/build

tools/llvm-dis/CMakeFiles/llvm-dis.dir/requires: tools/llvm-dis/CMakeFiles/llvm-dis.dir/llvm-dis.cpp.o.requires

.PHONY : tools/llvm-dis/CMakeFiles/llvm-dis.dir/requires

tools/llvm-dis/CMakeFiles/llvm-dis.dir/clean:
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/llvm-dis && $(CMAKE_COMMAND) -P CMakeFiles/llvm-dis.dir/cmake_clean.cmake
.PHONY : tools/llvm-dis/CMakeFiles/llvm-dis.dir/clean

tools/llvm-dis/CMakeFiles/llvm-dis.dir/depend:
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/tools/llvm-dis /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64 /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/llvm-dis /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/llvm-dis/CMakeFiles/llvm-dis.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tools/llvm-dis/CMakeFiles/llvm-dis.dir/depend

