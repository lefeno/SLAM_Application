# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/blue/Desktop/SLAM/test_server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/blue/Desktop/SLAM/test_server/build

# Include any dependencies generated for this target.
include src/CMakeFiles/main.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/main.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/main.dir/flags.make

src/CMakeFiles/main.dir/test.cpp.o: src/CMakeFiles/main.dir/flags.make
src/CMakeFiles/main.dir/test.cpp.o: ../src/test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/blue/Desktop/SLAM/test_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/main.dir/test.cpp.o"
	cd /home/blue/Desktop/SLAM/test_server/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/test.cpp.o -c /home/blue/Desktop/SLAM/test_server/src/test.cpp

src/CMakeFiles/main.dir/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/test.cpp.i"
	cd /home/blue/Desktop/SLAM/test_server/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/blue/Desktop/SLAM/test_server/src/test.cpp > CMakeFiles/main.dir/test.cpp.i

src/CMakeFiles/main.dir/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/test.cpp.s"
	cd /home/blue/Desktop/SLAM/test_server/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/blue/Desktop/SLAM/test_server/src/test.cpp -o CMakeFiles/main.dir/test.cpp.s

src/CMakeFiles/main.dir/jsoncpp.cpp.o: src/CMakeFiles/main.dir/flags.make
src/CMakeFiles/main.dir/jsoncpp.cpp.o: ../src/jsoncpp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/blue/Desktop/SLAM/test_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/main.dir/jsoncpp.cpp.o"
	cd /home/blue/Desktop/SLAM/test_server/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/jsoncpp.cpp.o -c /home/blue/Desktop/SLAM/test_server/src/jsoncpp.cpp

src/CMakeFiles/main.dir/jsoncpp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/jsoncpp.cpp.i"
	cd /home/blue/Desktop/SLAM/test_server/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/blue/Desktop/SLAM/test_server/src/jsoncpp.cpp > CMakeFiles/main.dir/jsoncpp.cpp.i

src/CMakeFiles/main.dir/jsoncpp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/jsoncpp.cpp.s"
	cd /home/blue/Desktop/SLAM/test_server/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/blue/Desktop/SLAM/test_server/src/jsoncpp.cpp -o CMakeFiles/main.dir/jsoncpp.cpp.s

src/CMakeFiles/main.dir/utils.cpp.o: src/CMakeFiles/main.dir/flags.make
src/CMakeFiles/main.dir/utils.cpp.o: ../src/utils.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/blue/Desktop/SLAM/test_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/CMakeFiles/main.dir/utils.cpp.o"
	cd /home/blue/Desktop/SLAM/test_server/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/utils.cpp.o -c /home/blue/Desktop/SLAM/test_server/src/utils.cpp

src/CMakeFiles/main.dir/utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/utils.cpp.i"
	cd /home/blue/Desktop/SLAM/test_server/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/blue/Desktop/SLAM/test_server/src/utils.cpp > CMakeFiles/main.dir/utils.cpp.i

src/CMakeFiles/main.dir/utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/utils.cpp.s"
	cd /home/blue/Desktop/SLAM/test_server/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/blue/Desktop/SLAM/test_server/src/utils.cpp -o CMakeFiles/main.dir/utils.cpp.s

src/CMakeFiles/main.dir/Mediator.cpp.o: src/CMakeFiles/main.dir/flags.make
src/CMakeFiles/main.dir/Mediator.cpp.o: ../src/Mediator.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/blue/Desktop/SLAM/test_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/CMakeFiles/main.dir/Mediator.cpp.o"
	cd /home/blue/Desktop/SLAM/test_server/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/Mediator.cpp.o -c /home/blue/Desktop/SLAM/test_server/src/Mediator.cpp

src/CMakeFiles/main.dir/Mediator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/Mediator.cpp.i"
	cd /home/blue/Desktop/SLAM/test_server/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/blue/Desktop/SLAM/test_server/src/Mediator.cpp > CMakeFiles/main.dir/Mediator.cpp.i

src/CMakeFiles/main.dir/Mediator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/Mediator.cpp.s"
	cd /home/blue/Desktop/SLAM/test_server/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/blue/Desktop/SLAM/test_server/src/Mediator.cpp -o CMakeFiles/main.dir/Mediator.cpp.s

src/CMakeFiles/main.dir/Employee.cpp.o: src/CMakeFiles/main.dir/flags.make
src/CMakeFiles/main.dir/Employee.cpp.o: ../src/Employee.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/blue/Desktop/SLAM/test_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/CMakeFiles/main.dir/Employee.cpp.o"
	cd /home/blue/Desktop/SLAM/test_server/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/Employee.cpp.o -c /home/blue/Desktop/SLAM/test_server/src/Employee.cpp

src/CMakeFiles/main.dir/Employee.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/Employee.cpp.i"
	cd /home/blue/Desktop/SLAM/test_server/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/blue/Desktop/SLAM/test_server/src/Employee.cpp > CMakeFiles/main.dir/Employee.cpp.i

src/CMakeFiles/main.dir/Employee.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/Employee.cpp.s"
	cd /home/blue/Desktop/SLAM/test_server/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/blue/Desktop/SLAM/test_server/src/Employee.cpp -o CMakeFiles/main.dir/Employee.cpp.s

# Object files for target main
main_OBJECTS = \
"CMakeFiles/main.dir/test.cpp.o" \
"CMakeFiles/main.dir/jsoncpp.cpp.o" \
"CMakeFiles/main.dir/utils.cpp.o" \
"CMakeFiles/main.dir/Mediator.cpp.o" \
"CMakeFiles/main.dir/Employee.cpp.o"

# External object files for target main
main_EXTERNAL_OBJECTS =

src/main: src/CMakeFiles/main.dir/test.cpp.o
src/main: src/CMakeFiles/main.dir/jsoncpp.cpp.o
src/main: src/CMakeFiles/main.dir/utils.cpp.o
src/main: src/CMakeFiles/main.dir/Mediator.cpp.o
src/main: src/CMakeFiles/main.dir/Employee.cpp.o
src/main: src/CMakeFiles/main.dir/build.make
src/main: /usr/lib/x86_64-linux-gnu/libboost_program_options.a
src/main: /usr/lib/x86_64-linux-gnu/libboost_thread.a
src/main: /usr/lib/x86_64-linux-gnu/libboost_chrono.a
src/main: /usr/lib/x86_64-linux-gnu/libboost_system.a
src/main: /usr/lib/x86_64-linux-gnu/libboost_date_time.a
src/main: /usr/lib/x86_64-linux-gnu/libboost_atomic.a
src/main: src/CMakeFiles/main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/blue/Desktop/SLAM/test_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable main"
	cd /home/blue/Desktop/SLAM/test_server/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/main.dir/build: src/main

.PHONY : src/CMakeFiles/main.dir/build

src/CMakeFiles/main.dir/clean:
	cd /home/blue/Desktop/SLAM/test_server/build/src && $(CMAKE_COMMAND) -P CMakeFiles/main.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/main.dir/clean

src/CMakeFiles/main.dir/depend:
	cd /home/blue/Desktop/SLAM/test_server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/blue/Desktop/SLAM/test_server /home/blue/Desktop/SLAM/test_server/src /home/blue/Desktop/SLAM/test_server/build /home/blue/Desktop/SLAM/test_server/build/src /home/blue/Desktop/SLAM/test_server/build/src/CMakeFiles/main.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/main.dir/depend

