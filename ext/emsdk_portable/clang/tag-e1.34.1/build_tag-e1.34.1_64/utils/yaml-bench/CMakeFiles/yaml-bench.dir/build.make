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
include utils/yaml-bench/CMakeFiles/yaml-bench.dir/depend.make

# Include the progress variables for this target.
include utils/yaml-bench/CMakeFiles/yaml-bench.dir/progress.make

# Include the compile flags for this target's objects.
include utils/yaml-bench/CMakeFiles/yaml-bench.dir/flags.make

utils/yaml-bench/CMakeFiles/yaml-bench.dir/YAMLBench.cpp.o: utils/yaml-bench/CMakeFiles/yaml-bench.dir/flags.make
utils/yaml-bench/CMakeFiles/yaml-bench.dir/YAMLBench.cpp.o: /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/utils/yaml-bench/YAMLBench.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object utils/yaml-bench/CMakeFiles/yaml-bench.dir/YAMLBench.cpp.o"
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/utils/yaml-bench && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/yaml-bench.dir/YAMLBench.cpp.o -c /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/utils/yaml-bench/YAMLBench.cpp

utils/yaml-bench/CMakeFiles/yaml-bench.dir/YAMLBench.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/yaml-bench.dir/YAMLBench.cpp.i"
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/utils/yaml-bench && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/utils/yaml-bench/YAMLBench.cpp > CMakeFiles/yaml-bench.dir/YAMLBench.cpp.i

utils/yaml-bench/CMakeFiles/yaml-bench.dir/YAMLBench.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/yaml-bench.dir/YAMLBench.cpp.s"
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/utils/yaml-bench && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/utils/yaml-bench/YAMLBench.cpp -o CMakeFiles/yaml-bench.dir/YAMLBench.cpp.s

utils/yaml-bench/CMakeFiles/yaml-bench.dir/YAMLBench.cpp.o.requires:

.PHONY : utils/yaml-bench/CMakeFiles/yaml-bench.dir/YAMLBench.cpp.o.requires

utils/yaml-bench/CMakeFiles/yaml-bench.dir/YAMLBench.cpp.o.provides: utils/yaml-bench/CMakeFiles/yaml-bench.dir/YAMLBench.cpp.o.requires
	$(MAKE) -f utils/yaml-bench/CMakeFiles/yaml-bench.dir/build.make utils/yaml-bench/CMakeFiles/yaml-bench.dir/YAMLBench.cpp.o.provides.build
.PHONY : utils/yaml-bench/CMakeFiles/yaml-bench.dir/YAMLBench.cpp.o.provides

utils/yaml-bench/CMakeFiles/yaml-bench.dir/YAMLBench.cpp.o.provides.build: utils/yaml-bench/CMakeFiles/yaml-bench.dir/YAMLBench.cpp.o


# Object files for target yaml-bench
yaml__bench_OBJECTS = \
"CMakeFiles/yaml-bench.dir/YAMLBench.cpp.o"

# External object files for target yaml-bench
yaml__bench_EXTERNAL_OBJECTS =

bin/yaml-bench: utils/yaml-bench/CMakeFiles/yaml-bench.dir/YAMLBench.cpp.o
bin/yaml-bench: utils/yaml-bench/CMakeFiles/yaml-bench.dir/build.make
bin/yaml-bench: lib/libLLVMSupport.a
bin/yaml-bench: utils/yaml-bench/CMakeFiles/yaml-bench.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../bin/yaml-bench"
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/utils/yaml-bench && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/yaml-bench.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
utils/yaml-bench/CMakeFiles/yaml-bench.dir/build: bin/yaml-bench

.PHONY : utils/yaml-bench/CMakeFiles/yaml-bench.dir/build

utils/yaml-bench/CMakeFiles/yaml-bench.dir/requires: utils/yaml-bench/CMakeFiles/yaml-bench.dir/YAMLBench.cpp.o.requires

.PHONY : utils/yaml-bench/CMakeFiles/yaml-bench.dir/requires

utils/yaml-bench/CMakeFiles/yaml-bench.dir/clean:
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/utils/yaml-bench && $(CMAKE_COMMAND) -P CMakeFiles/yaml-bench.dir/cmake_clean.cmake
.PHONY : utils/yaml-bench/CMakeFiles/yaml-bench.dir/clean

utils/yaml-bench/CMakeFiles/yaml-bench.dir/depend:
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/utils/yaml-bench /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64 /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/utils/yaml-bench /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/utils/yaml-bench/CMakeFiles/yaml-bench.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : utils/yaml-bench/CMakeFiles/yaml-bench.dir/depend

