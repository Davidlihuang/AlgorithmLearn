# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake-3.23.1-linux-x86_64/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake-3.23.1-linux-x86_64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/huangli/MyLearning/AlgorithmLearn/myrouter/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/huangli/MyLearning/AlgorithmLearn/myrouter/src/build

# Include any dependencies generated for this target.
include CMakeFiles/pcbRouter.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/pcbRouter.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/pcbRouter.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/pcbRouter.dir/flags.make

CMakeFiles/pcbRouter.dir/main.cpp.o: CMakeFiles/pcbRouter.dir/flags.make
CMakeFiles/pcbRouter.dir/main.cpp.o: ../main.cpp
CMakeFiles/pcbRouter.dir/main.cpp.o: CMakeFiles/pcbRouter.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/huangli/MyLearning/AlgorithmLearn/myrouter/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/pcbRouter.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/pcbRouter.dir/main.cpp.o -MF CMakeFiles/pcbRouter.dir/main.cpp.o.d -o CMakeFiles/pcbRouter.dir/main.cpp.o -c /home/huangli/MyLearning/AlgorithmLearn/myrouter/src/main.cpp

CMakeFiles/pcbRouter.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pcbRouter.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/huangli/MyLearning/AlgorithmLearn/myrouter/src/main.cpp > CMakeFiles/pcbRouter.dir/main.cpp.i

CMakeFiles/pcbRouter.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pcbRouter.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/huangli/MyLearning/AlgorithmLearn/myrouter/src/main.cpp -o CMakeFiles/pcbRouter.dir/main.cpp.s

CMakeFiles/pcbRouter.dir/dsnparser.cpp.o: CMakeFiles/pcbRouter.dir/flags.make
CMakeFiles/pcbRouter.dir/dsnparser.cpp.o: ../dsnparser.cpp
CMakeFiles/pcbRouter.dir/dsnparser.cpp.o: CMakeFiles/pcbRouter.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/huangli/MyLearning/AlgorithmLearn/myrouter/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/pcbRouter.dir/dsnparser.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/pcbRouter.dir/dsnparser.cpp.o -MF CMakeFiles/pcbRouter.dir/dsnparser.cpp.o.d -o CMakeFiles/pcbRouter.dir/dsnparser.cpp.o -c /home/huangli/MyLearning/AlgorithmLearn/myrouter/src/dsnparser.cpp

CMakeFiles/pcbRouter.dir/dsnparser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pcbRouter.dir/dsnparser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/huangli/MyLearning/AlgorithmLearn/myrouter/src/dsnparser.cpp > CMakeFiles/pcbRouter.dir/dsnparser.cpp.i

CMakeFiles/pcbRouter.dir/dsnparser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pcbRouter.dir/dsnparser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/huangli/MyLearning/AlgorithmLearn/myrouter/src/dsnparser.cpp -o CMakeFiles/pcbRouter.dir/dsnparser.cpp.s

CMakeFiles/pcbRouter.dir/mymath.cpp.o: CMakeFiles/pcbRouter.dir/flags.make
CMakeFiles/pcbRouter.dir/mymath.cpp.o: ../mymath.cpp
CMakeFiles/pcbRouter.dir/mymath.cpp.o: CMakeFiles/pcbRouter.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/huangli/MyLearning/AlgorithmLearn/myrouter/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/pcbRouter.dir/mymath.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/pcbRouter.dir/mymath.cpp.o -MF CMakeFiles/pcbRouter.dir/mymath.cpp.o.d -o CMakeFiles/pcbRouter.dir/mymath.cpp.o -c /home/huangli/MyLearning/AlgorithmLearn/myrouter/src/mymath.cpp

CMakeFiles/pcbRouter.dir/mymath.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pcbRouter.dir/mymath.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/huangli/MyLearning/AlgorithmLearn/myrouter/src/mymath.cpp > CMakeFiles/pcbRouter.dir/mymath.cpp.i

CMakeFiles/pcbRouter.dir/mymath.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pcbRouter.dir/mymath.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/huangli/MyLearning/AlgorithmLearn/myrouter/src/mymath.cpp -o CMakeFiles/pcbRouter.dir/mymath.cpp.s

# Object files for target pcbRouter
pcbRouter_OBJECTS = \
"CMakeFiles/pcbRouter.dir/main.cpp.o" \
"CMakeFiles/pcbRouter.dir/dsnparser.cpp.o" \
"CMakeFiles/pcbRouter.dir/mymath.cpp.o"

# External object files for target pcbRouter
pcbRouter_EXTERNAL_OBJECTS =

pcbRouter: CMakeFiles/pcbRouter.dir/main.cpp.o
pcbRouter: CMakeFiles/pcbRouter.dir/dsnparser.cpp.o
pcbRouter: CMakeFiles/pcbRouter.dir/mymath.cpp.o
pcbRouter: CMakeFiles/pcbRouter.dir/build.make
pcbRouter: CMakeFiles/pcbRouter.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/huangli/MyLearning/AlgorithmLearn/myrouter/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable pcbRouter"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pcbRouter.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/pcbRouter.dir/build: pcbRouter
.PHONY : CMakeFiles/pcbRouter.dir/build

CMakeFiles/pcbRouter.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/pcbRouter.dir/cmake_clean.cmake
.PHONY : CMakeFiles/pcbRouter.dir/clean

CMakeFiles/pcbRouter.dir/depend:
	cd /home/huangli/MyLearning/AlgorithmLearn/myrouter/src/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/huangli/MyLearning/AlgorithmLearn/myrouter/src /home/huangli/MyLearning/AlgorithmLearn/myrouter/src /home/huangli/MyLearning/AlgorithmLearn/myrouter/src/build /home/huangli/MyLearning/AlgorithmLearn/myrouter/src/build /home/huangli/MyLearning/AlgorithmLearn/myrouter/src/build/CMakeFiles/pcbRouter.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/pcbRouter.dir/depend

