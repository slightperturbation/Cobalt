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

# Utility rule file for ClangCommentCommandList.

# Include the progress variables for this target.
include tools/clang/include/clang/AST/CMakeFiles/ClangCommentCommandList.dir/progress.make

tools/clang/include/clang/AST/CMakeFiles/ClangCommentCommandList: tools/clang/include/clang/AST/CommentCommandList.inc


tools/clang/include/clang/AST/CommentCommandList.inc: tools/clang/include/clang/AST/CommentCommandList.inc.tmp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Updating CommentCommandList.inc..."
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/include/clang/AST && /usr/local/Cellar/cmake/3.3.1/bin/cmake -E copy_if_different /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/include/clang/AST/CommentCommandList.inc.tmp /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/include/clang/AST/CommentCommandList.inc

tools/clang/include/clang/AST/CommentCommandList.inc.tmp: bin/clang-tblgen
tools/clang/include/clang/AST/CommentCommandList.inc.tmp: /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/include/clang/AST/CommentCommands.td
tools/clang/include/clang/AST/CommentCommandList.inc.tmp: /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/include/clang/AST/CommentHTMLNamedCharacterReferences.td
tools/clang/include/clang/AST/CommentCommandList.inc.tmp: /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/include/clang/AST/CommentHTMLTags.td
tools/clang/include/clang/AST/CommentCommandList.inc.tmp: /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/CodeGen/ValueTypes.td
tools/clang/include/clang/AST/CommentCommandList.inc.tmp: /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/IR/Intrinsics.td
tools/clang/include/clang/AST/CommentCommandList.inc.tmp: /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/IR/IntrinsicsAArch64.td
tools/clang/include/clang/AST/CommentCommandList.inc.tmp: /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/IR/IntrinsicsARM.td
tools/clang/include/clang/AST/CommentCommandList.inc.tmp: /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/IR/IntrinsicsBPF.td
tools/clang/include/clang/AST/CommentCommandList.inc.tmp: /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/IR/IntrinsicsHexagon.td
tools/clang/include/clang/AST/CommentCommandList.inc.tmp: /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/IR/IntrinsicsMips.td
tools/clang/include/clang/AST/CommentCommandList.inc.tmp: /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/IR/IntrinsicsNVVM.td
tools/clang/include/clang/AST/CommentCommandList.inc.tmp: /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/IR/IntrinsicsPowerPC.td
tools/clang/include/clang/AST/CommentCommandList.inc.tmp: /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/IR/IntrinsicsR600.td
tools/clang/include/clang/AST/CommentCommandList.inc.tmp: /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/IR/IntrinsicsSystemZ.td
tools/clang/include/clang/AST/CommentCommandList.inc.tmp: /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/IR/IntrinsicsX86.td
tools/clang/include/clang/AST/CommentCommandList.inc.tmp: /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/IR/IntrinsicsXCore.td
tools/clang/include/clang/AST/CommentCommandList.inc.tmp: /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/Option/OptParser.td
tools/clang/include/clang/AST/CommentCommandList.inc.tmp: /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/Target/Target.td
tools/clang/include/clang/AST/CommentCommandList.inc.tmp: /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/Target/TargetCallingConv.td
tools/clang/include/clang/AST/CommentCommandList.inc.tmp: /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/Target/TargetItinerary.td
tools/clang/include/clang/AST/CommentCommandList.inc.tmp: /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/Target/TargetSchedule.td
tools/clang/include/clang/AST/CommentCommandList.inc.tmp: /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/include/llvm/Target/TargetSelectionDAG.td
tools/clang/include/clang/AST/CommentCommandList.inc.tmp: /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/include/clang/AST/CommentCommands.td
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CommentCommandList.inc..."
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/include/clang/AST && ../../../../../bin/clang-tblgen -gen-clang-comment-command-list -I /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/include/clang/AST -I /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/lib/Target -I /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/include /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/include/clang/AST/CommentCommands.td -o /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/include/clang/AST/CommentCommandList.inc.tmp

ClangCommentCommandList: tools/clang/include/clang/AST/CMakeFiles/ClangCommentCommandList
ClangCommentCommandList: tools/clang/include/clang/AST/CommentCommandList.inc
ClangCommentCommandList: tools/clang/include/clang/AST/CommentCommandList.inc.tmp
ClangCommentCommandList: tools/clang/include/clang/AST/CMakeFiles/ClangCommentCommandList.dir/build.make

.PHONY : ClangCommentCommandList

# Rule to build all files generated by this target.
tools/clang/include/clang/AST/CMakeFiles/ClangCommentCommandList.dir/build: ClangCommentCommandList

.PHONY : tools/clang/include/clang/AST/CMakeFiles/ClangCommentCommandList.dir/build

tools/clang/include/clang/AST/CMakeFiles/ClangCommentCommandList.dir/clean:
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/include/clang/AST && $(CMAKE_COMMAND) -P CMakeFiles/ClangCommentCommandList.dir/cmake_clean.cmake
.PHONY : tools/clang/include/clang/AST/CMakeFiles/ClangCommentCommandList.dir/clean

tools/clang/include/clang/AST/CMakeFiles/ClangCommentCommandList.dir/depend:
	cd /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/src/tools/clang/include/clang/AST /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64 /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/include/clang/AST /Users/vector/tmp/Cobalt/ext/emsdk_portable/clang/tag-e1.34.1/build_tag-e1.34.1_64/tools/clang/include/clang/AST/CMakeFiles/ClangCommentCommandList.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tools/clang/include/clang/AST/CMakeFiles/ClangCommentCommandList.dir/depend

