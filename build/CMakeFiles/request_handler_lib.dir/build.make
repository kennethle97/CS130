# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/CS130/Team-404

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/CS130/Team-404/build

# Include any dependencies generated for this target.
include CMakeFiles/request_handler_lib.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/request_handler_lib.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/request_handler_lib.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/request_handler_lib.dir/flags.make

CMakeFiles/request_handler_lib.dir/src/request_handler_dispatcher.cc.o: CMakeFiles/request_handler_lib.dir/flags.make
CMakeFiles/request_handler_lib.dir/src/request_handler_dispatcher.cc.o: ../src/request_handler_dispatcher.cc
CMakeFiles/request_handler_lib.dir/src/request_handler_dispatcher.cc.o: CMakeFiles/request_handler_lib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/CS130/Team-404/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/request_handler_lib.dir/src/request_handler_dispatcher.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/request_handler_lib.dir/src/request_handler_dispatcher.cc.o -MF CMakeFiles/request_handler_lib.dir/src/request_handler_dispatcher.cc.o.d -o CMakeFiles/request_handler_lib.dir/src/request_handler_dispatcher.cc.o -c /root/CS130/Team-404/src/request_handler_dispatcher.cc

CMakeFiles/request_handler_lib.dir/src/request_handler_dispatcher.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/request_handler_lib.dir/src/request_handler_dispatcher.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/CS130/Team-404/src/request_handler_dispatcher.cc > CMakeFiles/request_handler_lib.dir/src/request_handler_dispatcher.cc.i

CMakeFiles/request_handler_lib.dir/src/request_handler_dispatcher.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/request_handler_lib.dir/src/request_handler_dispatcher.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/CS130/Team-404/src/request_handler_dispatcher.cc -o CMakeFiles/request_handler_lib.dir/src/request_handler_dispatcher.cc.s

CMakeFiles/request_handler_lib.dir/src/request_handler/request_handler_echo.cc.o: CMakeFiles/request_handler_lib.dir/flags.make
CMakeFiles/request_handler_lib.dir/src/request_handler/request_handler_echo.cc.o: ../src/request_handler/request_handler_echo.cc
CMakeFiles/request_handler_lib.dir/src/request_handler/request_handler_echo.cc.o: CMakeFiles/request_handler_lib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/CS130/Team-404/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/request_handler_lib.dir/src/request_handler/request_handler_echo.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/request_handler_lib.dir/src/request_handler/request_handler_echo.cc.o -MF CMakeFiles/request_handler_lib.dir/src/request_handler/request_handler_echo.cc.o.d -o CMakeFiles/request_handler_lib.dir/src/request_handler/request_handler_echo.cc.o -c /root/CS130/Team-404/src/request_handler/request_handler_echo.cc

CMakeFiles/request_handler_lib.dir/src/request_handler/request_handler_echo.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/request_handler_lib.dir/src/request_handler/request_handler_echo.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/CS130/Team-404/src/request_handler/request_handler_echo.cc > CMakeFiles/request_handler_lib.dir/src/request_handler/request_handler_echo.cc.i

CMakeFiles/request_handler_lib.dir/src/request_handler/request_handler_echo.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/request_handler_lib.dir/src/request_handler/request_handler_echo.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/CS130/Team-404/src/request_handler/request_handler_echo.cc -o CMakeFiles/request_handler_lib.dir/src/request_handler/request_handler_echo.cc.s

CMakeFiles/request_handler_lib.dir/src/request_handler/request_handler_static.cc.o: CMakeFiles/request_handler_lib.dir/flags.make
CMakeFiles/request_handler_lib.dir/src/request_handler/request_handler_static.cc.o: ../src/request_handler/request_handler_static.cc
CMakeFiles/request_handler_lib.dir/src/request_handler/request_handler_static.cc.o: CMakeFiles/request_handler_lib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/CS130/Team-404/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/request_handler_lib.dir/src/request_handler/request_handler_static.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/request_handler_lib.dir/src/request_handler/request_handler_static.cc.o -MF CMakeFiles/request_handler_lib.dir/src/request_handler/request_handler_static.cc.o.d -o CMakeFiles/request_handler_lib.dir/src/request_handler/request_handler_static.cc.o -c /root/CS130/Team-404/src/request_handler/request_handler_static.cc

CMakeFiles/request_handler_lib.dir/src/request_handler/request_handler_static.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/request_handler_lib.dir/src/request_handler/request_handler_static.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/CS130/Team-404/src/request_handler/request_handler_static.cc > CMakeFiles/request_handler_lib.dir/src/request_handler/request_handler_static.cc.i

CMakeFiles/request_handler_lib.dir/src/request_handler/request_handler_static.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/request_handler_lib.dir/src/request_handler/request_handler_static.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/CS130/Team-404/src/request_handler/request_handler_static.cc -o CMakeFiles/request_handler_lib.dir/src/request_handler/request_handler_static.cc.s

# Object files for target request_handler_lib
request_handler_lib_OBJECTS = \
"CMakeFiles/request_handler_lib.dir/src/request_handler_dispatcher.cc.o" \
"CMakeFiles/request_handler_lib.dir/src/request_handler/request_handler_echo.cc.o" \
"CMakeFiles/request_handler_lib.dir/src/request_handler/request_handler_static.cc.o"

# External object files for target request_handler_lib
request_handler_lib_EXTERNAL_OBJECTS =

librequest_handler_lib.a: CMakeFiles/request_handler_lib.dir/src/request_handler_dispatcher.cc.o
librequest_handler_lib.a: CMakeFiles/request_handler_lib.dir/src/request_handler/request_handler_echo.cc.o
librequest_handler_lib.a: CMakeFiles/request_handler_lib.dir/src/request_handler/request_handler_static.cc.o
librequest_handler_lib.a: CMakeFiles/request_handler_lib.dir/build.make
librequest_handler_lib.a: CMakeFiles/request_handler_lib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/CS130/Team-404/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library librequest_handler_lib.a"
	$(CMAKE_COMMAND) -P CMakeFiles/request_handler_lib.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/request_handler_lib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/request_handler_lib.dir/build: librequest_handler_lib.a
.PHONY : CMakeFiles/request_handler_lib.dir/build

CMakeFiles/request_handler_lib.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/request_handler_lib.dir/cmake_clean.cmake
.PHONY : CMakeFiles/request_handler_lib.dir/clean

CMakeFiles/request_handler_lib.dir/depend:
	cd /root/CS130/Team-404/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/CS130/Team-404 /root/CS130/Team-404 /root/CS130/Team-404/build /root/CS130/Team-404/build /root/CS130/Team-404/build/CMakeFiles/request_handler_lib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/request_handler_lib.dir/depend

