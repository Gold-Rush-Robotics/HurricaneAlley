# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

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
CMAKE_COMMAND = /snap/cmake/1158/bin/cmake

# The command to remove a file.
RM = /snap/cmake/1158/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pi/2023/PiPCA9685/examples/cpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/2023/PiPCA9685/examples/cpp/build

# Include any dependencies generated for this target.
include CMakeFiles/example-cpp.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/example-cpp.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/example-cpp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/example-cpp.dir/flags.make

CMakeFiles/example-cpp.dir/main.cpp.o: CMakeFiles/example-cpp.dir/flags.make
CMakeFiles/example-cpp.dir/main.cpp.o: /home/pi/2023/PiPCA9685/examples/cpp/main.cpp
CMakeFiles/example-cpp.dir/main.cpp.o: CMakeFiles/example-cpp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/2023/PiPCA9685/examples/cpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/example-cpp.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/example-cpp.dir/main.cpp.o -MF CMakeFiles/example-cpp.dir/main.cpp.o.d -o CMakeFiles/example-cpp.dir/main.cpp.o -c /home/pi/2023/PiPCA9685/examples/cpp/main.cpp

CMakeFiles/example-cpp.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/example-cpp.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/2023/PiPCA9685/examples/cpp/main.cpp > CMakeFiles/example-cpp.dir/main.cpp.i

CMakeFiles/example-cpp.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/example-cpp.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/2023/PiPCA9685/examples/cpp/main.cpp -o CMakeFiles/example-cpp.dir/main.cpp.s

# Object files for target example-cpp
example__cpp_OBJECTS = \
"CMakeFiles/example-cpp.dir/main.cpp.o"

# External object files for target example-cpp
example__cpp_EXTERNAL_OBJECTS =

example-cpp: CMakeFiles/example-cpp.dir/main.cpp.o
example-cpp: CMakeFiles/example-cpp.dir/build.make
example-cpp: CMakeFiles/example-cpp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pi/2023/PiPCA9685/examples/cpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable example-cpp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/example-cpp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/example-cpp.dir/build: example-cpp
.PHONY : CMakeFiles/example-cpp.dir/build

CMakeFiles/example-cpp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/example-cpp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/example-cpp.dir/clean

CMakeFiles/example-cpp.dir/depend:
	cd /home/pi/2023/PiPCA9685/examples/cpp/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/2023/PiPCA9685/examples/cpp /home/pi/2023/PiPCA9685/examples/cpp /home/pi/2023/PiPCA9685/examples/cpp/build /home/pi/2023/PiPCA9685/examples/cpp/build /home/pi/2023/PiPCA9685/examples/cpp/build/CMakeFiles/example-cpp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/example-cpp.dir/depend
