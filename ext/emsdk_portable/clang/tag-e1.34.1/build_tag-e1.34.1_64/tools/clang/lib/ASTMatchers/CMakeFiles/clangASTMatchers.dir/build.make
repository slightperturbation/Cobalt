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
include tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/depend.make

# Include the progress variables for this target.
include tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/progress.make

# Include the compile flags for this target's objects.
include tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/flags.make

tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/ASTMatchFinder.cpp.o: tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/flags.make
tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/ASTMatchFinder.cpp.o: /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/lib/ASTMatchers/ASTMatchFinder.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/ASTMatchFinder.cpp.o"
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/lib/ASTMatchers && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/clangASTMatchers.dir/ASTMatchFinder.cpp.o -c /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/lib/ASTMatchers/ASTMatchFinder.cpp

tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/ASTMatchFinder.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/clangASTMatchers.dir/ASTMatchFinder.cpp.i"
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/lib/ASTMatchers && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/lib/ASTMatchers/ASTMatchFinder.cpp > CMakeFiles/clangASTMatchers.dir/ASTMatchFinder.cpp.i

tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/ASTMatchFinder.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/clangASTMatchers.dir/ASTMatchFinder.cpp.s"
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/lib/ASTMatchers && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/lib/ASTMatchers/ASTMatchFinder.cpp -o CMakeFiles/clangASTMatchers.dir/ASTMatchFinder.cpp.s

tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/ASTMatchFinder.cpp.o.requires:

.PHONY : tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/ASTMatchFinder.cpp.o.requires

tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/ASTMatchFinder.cpp.o.provides: tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/ASTMatchFinder.cpp.o.requires
	$(MAKE) -f tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/build.make tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/ASTMatchFinder.cpp.o.provides.build
.PHONY : tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/ASTMatchFinder.cpp.o.provides

tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/ASTMatchFinder.cpp.o.provides.build: tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/ASTMatchFinder.cpp.o


tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/ASTMatchersInternal.cpp.o: tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/flags.make
tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/ASTMatchersInternal.cpp.o: /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/lib/ASTMatchers/ASTMatchersInternal.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/ASTMatchersInternal.cpp.o"
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/lib/ASTMatchers && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/clangASTMatchers.dir/ASTMatchersInternal.cpp.o -c /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/lib/ASTMatchers/ASTMatchersInternal.cpp

tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/ASTMatchersInternal.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/clangASTMatchers.dir/ASTMatchersInternal.cpp.i"
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/lib/ASTMatchers && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/lib/ASTMatchers/ASTMatchersInternal.cpp > CMakeFiles/clangASTMatchers.dir/ASTMatchersInternal.cpp.i

tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/ASTMatchersInternal.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/clangASTMatchers.dir/ASTMatchersInternal.cpp.s"
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/lib/ASTMatchers && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/lib/ASTMatchers/ASTMatchersInternal.cpp -o CMakeFiles/clangASTMatchers.dir/ASTMatchersInternal.cpp.s

tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/ASTMatchersInternal.cpp.o.requires:

.PHONY : tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/ASTMatchersInternal.cpp.o.requires

tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/ASTMatchersInternal.cpp.o.provides: tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/ASTMatchersInternal.cpp.o.requires
	$(MAKE) -f tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/build.make tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/ASTMatchersInternal.cpp.o.provides.build
.PHONY : tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/ASTMatchersInternal.cpp.o.provides

tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/ASTMatchersInternal.cpp.o.provides.build: tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/ASTMatchersInternal.cpp.o


# Object files for target clangASTMatchers
clangASTMatchers_OBJECTS = \
"CMakeFiles/clangASTMatchers.dir/ASTMatchFinder.cpp.o" \
"CMakeFiles/clangASTMatchers.dir/ASTMatchersInternal.cpp.o"

# External object files for target clangASTMatchers
clangASTMatchers_EXTERNAL_OBJECTS =

lib/libclangASTMatchers.a: tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/ASTMatchFinder.cpp.o
lib/libclangASTMatchers.a: tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/ASTMatchersInternal.cpp.o
lib/libclangASTMatchers.a: tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/build.make
lib/libclangASTMatchers.a: tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library ../../../../lib/libclangASTMatchers.a"
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/lib/ASTMatchers && $(CMAKE_COMMAND) -P CMakeFiles/clangASTMatchers.dir/cmake_clean_target.cmake
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/lib/ASTMatchers && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/clangASTMatchers.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/build: lib/libclangASTMatchers.a

.PHONY : tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/build

tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/requires: tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/ASTMatchFinder.cpp.o.requires
tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/requires: tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/ASTMatchersInternal.cpp.o.requires

.PHONY : tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/requires

tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/clean:
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/lib/ASTMatchers && $(CMAKE_COMMAND) -P CMakeFiles/clangASTMatchers.dir/cmake_clean.cmake
.PHONY : tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/clean

tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/depend:
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/lib/ASTMatchers /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64 /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/lib/ASTMatchers /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tools/clang/lib/ASTMatchers/CMakeFiles/clangASTMatchers.dir/depend
