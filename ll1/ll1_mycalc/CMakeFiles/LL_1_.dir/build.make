# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.15.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.15.2/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/liangyulin/Workspace/Dev/VSCodeProjects/CompilerTheory/ll1/ll1_mycalc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/liangyulin/Workspace/Dev/VSCodeProjects/CompilerTheory/ll1/ll1_mycalc

# Include any dependencies generated for this target.
include CMakeFiles/LL_1_.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/LL_1_.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/LL_1_.dir/flags.make

CMakeFiles/LL_1_.dir/main.cc.o: CMakeFiles/LL_1_.dir/flags.make
CMakeFiles/LL_1_.dir/main.cc.o: main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/liangyulin/Workspace/Dev/VSCodeProjects/CompilerTheory/ll1/ll1_mycalc/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/LL_1_.dir/main.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LL_1_.dir/main.cc.o -c /Users/liangyulin/Workspace/Dev/VSCodeProjects/CompilerTheory/ll1/ll1_mycalc/main.cc

CMakeFiles/LL_1_.dir/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LL_1_.dir/main.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/liangyulin/Workspace/Dev/VSCodeProjects/CompilerTheory/ll1/ll1_mycalc/main.cc > CMakeFiles/LL_1_.dir/main.cc.i

CMakeFiles/LL_1_.dir/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LL_1_.dir/main.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/liangyulin/Workspace/Dev/VSCodeProjects/CompilerTheory/ll1/ll1_mycalc/main.cc -o CMakeFiles/LL_1_.dir/main.cc.s

CMakeFiles/LL_1_.dir/lexer.cc.o: CMakeFiles/LL_1_.dir/flags.make
CMakeFiles/LL_1_.dir/lexer.cc.o: lexer.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/liangyulin/Workspace/Dev/VSCodeProjects/CompilerTheory/ll1/ll1_mycalc/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/LL_1_.dir/lexer.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LL_1_.dir/lexer.cc.o -c /Users/liangyulin/Workspace/Dev/VSCodeProjects/CompilerTheory/ll1/ll1_mycalc/lexer.cc

CMakeFiles/LL_1_.dir/lexer.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LL_1_.dir/lexer.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/liangyulin/Workspace/Dev/VSCodeProjects/CompilerTheory/ll1/ll1_mycalc/lexer.cc > CMakeFiles/LL_1_.dir/lexer.cc.i

CMakeFiles/LL_1_.dir/lexer.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LL_1_.dir/lexer.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/liangyulin/Workspace/Dev/VSCodeProjects/CompilerTheory/ll1/ll1_mycalc/lexer.cc -o CMakeFiles/LL_1_.dir/lexer.cc.s

# Object files for target LL_1_
LL_1__OBJECTS = \
"CMakeFiles/LL_1_.dir/main.cc.o" \
"CMakeFiles/LL_1_.dir/lexer.cc.o"

# External object files for target LL_1_
LL_1__EXTERNAL_OBJECTS =

LL_1_: CMakeFiles/LL_1_.dir/main.cc.o
LL_1_: CMakeFiles/LL_1_.dir/lexer.cc.o
LL_1_: CMakeFiles/LL_1_.dir/build.make
LL_1_: CMakeFiles/LL_1_.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/liangyulin/Workspace/Dev/VSCodeProjects/CompilerTheory/ll1/ll1_mycalc/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable LL_1_"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/LL_1_.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/LL_1_.dir/build: LL_1_

.PHONY : CMakeFiles/LL_1_.dir/build

CMakeFiles/LL_1_.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/LL_1_.dir/cmake_clean.cmake
.PHONY : CMakeFiles/LL_1_.dir/clean

CMakeFiles/LL_1_.dir/depend:
	cd /Users/liangyulin/Workspace/Dev/VSCodeProjects/CompilerTheory/ll1/ll1_mycalc && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/liangyulin/Workspace/Dev/VSCodeProjects/CompilerTheory/ll1/ll1_mycalc /Users/liangyulin/Workspace/Dev/VSCodeProjects/CompilerTheory/ll1/ll1_mycalc /Users/liangyulin/Workspace/Dev/VSCodeProjects/CompilerTheory/ll1/ll1_mycalc /Users/liangyulin/Workspace/Dev/VSCodeProjects/CompilerTheory/ll1/ll1_mycalc /Users/liangyulin/Workspace/Dev/VSCodeProjects/CompilerTheory/ll1/ll1_mycalc/CMakeFiles/LL_1_.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/LL_1_.dir/depend

