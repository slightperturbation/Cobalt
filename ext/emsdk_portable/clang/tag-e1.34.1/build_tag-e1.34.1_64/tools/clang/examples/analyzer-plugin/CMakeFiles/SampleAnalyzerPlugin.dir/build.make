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
include tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/depend.make

# Include the progress variables for this target.
include tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/progress.make

# Include the compile flags for this target's objects.
include tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/flags.make

tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/MainCallChecker.cpp.o: tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/flags.make
tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/MainCallChecker.cpp.o: /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/examples/analyzer-plugin/MainCallChecker.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/MainCallChecker.cpp.o"
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/examples/analyzer-plugin && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/SampleAnalyzerPlugin.dir/MainCallChecker.cpp.o -c /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/examples/analyzer-plugin/MainCallChecker.cpp

tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/MainCallChecker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SampleAnalyzerPlugin.dir/MainCallChecker.cpp.i"
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/examples/analyzer-plugin && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/examples/analyzer-plugin/MainCallChecker.cpp > CMakeFiles/SampleAnalyzerPlugin.dir/MainCallChecker.cpp.i

tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/MainCallChecker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SampleAnalyzerPlugin.dir/MainCallChecker.cpp.s"
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/examples/analyzer-plugin && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/examples/analyzer-plugin/MainCallChecker.cpp -o CMakeFiles/SampleAnalyzerPlugin.dir/MainCallChecker.cpp.s

tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/MainCallChecker.cpp.o.requires:

.PHONY : tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/MainCallChecker.cpp.o.requires

tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/MainCallChecker.cpp.o.provides: tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/MainCallChecker.cpp.o.requires
	$(MAKE) -f tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/build.make tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/MainCallChecker.cpp.o.provides.build
.PHONY : tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/MainCallChecker.cpp.o.provides

tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/MainCallChecker.cpp.o.provides.build: tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/MainCallChecker.cpp.o


# Object files for target SampleAnalyzerPlugin
SampleAnalyzerPlugin_OBJECTS = \
"CMakeFiles/SampleAnalyzerPlugin.dir/MainCallChecker.cpp.o"

# External object files for target SampleAnalyzerPlugin
SampleAnalyzerPlugin_EXTERNAL_OBJECTS =

lib/SampleAnalyzerPlugin.dylib: tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/MainCallChecker.cpp.o
lib/SampleAnalyzerPlugin.dylib: tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/build.make
lib/SampleAnalyzerPlugin.dylib: tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared module ../../../../lib/SampleAnalyzerPlugin.dylib"
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/examples/analyzer-plugin && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SampleAnalyzerPlugin.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/build: lib/SampleAnalyzerPlugin.dylib

.PHONY : tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/build

tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/requires: tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/MainCallChecker.cpp.o.requires

.PHONY : tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/requires

tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/clean:
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/examples/analyzer-plugin && $(CMAKE_COMMAND) -P CMakeFiles/SampleAnalyzerPlugin.dir/cmake_clean.cmake
.PHONY : tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/clean

tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/depend:
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/examples/analyzer-plugin /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64 /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/examples/analyzer-plugin /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tools/clang/examples/analyzer-plugin/CMakeFiles/SampleAnalyzerPlugin.dir/depend

