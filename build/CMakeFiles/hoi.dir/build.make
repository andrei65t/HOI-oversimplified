# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 4.0

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
CMAKE_COMMAND = /snap/cmake/1463/bin/cmake

# The command to remove a file.
RM = /snap/cmake/1463/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pia/Documents/HOI-oversimplified

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pia/Documents/HOI-oversimplified/build

# Include any dependencies generated for this target.
include CMakeFiles/hoi.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/hoi.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/hoi.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/hoi.dir/flags.make

CMakeFiles/hoi.dir/codegen:
.PHONY : CMakeFiles/hoi.dir/codegen

CMakeFiles/hoi.dir/src/main.cpp.o: CMakeFiles/hoi.dir/flags.make
CMakeFiles/hoi.dir/src/main.cpp.o: /home/pia/Documents/HOI-oversimplified/src/main.cpp
CMakeFiles/hoi.dir/src/main.cpp.o: CMakeFiles/hoi.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/pia/Documents/HOI-oversimplified/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/hoi.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/hoi.dir/src/main.cpp.o -MF CMakeFiles/hoi.dir/src/main.cpp.o.d -o CMakeFiles/hoi.dir/src/main.cpp.o -c /home/pia/Documents/HOI-oversimplified/src/main.cpp

CMakeFiles/hoi.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/hoi.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pia/Documents/HOI-oversimplified/src/main.cpp > CMakeFiles/hoi.dir/src/main.cpp.i

CMakeFiles/hoi.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/hoi.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pia/Documents/HOI-oversimplified/src/main.cpp -o CMakeFiles/hoi.dir/src/main.cpp.s

CMakeFiles/hoi.dir/src/init.cpp.o: CMakeFiles/hoi.dir/flags.make
CMakeFiles/hoi.dir/src/init.cpp.o: /home/pia/Documents/HOI-oversimplified/src/init.cpp
CMakeFiles/hoi.dir/src/init.cpp.o: CMakeFiles/hoi.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/pia/Documents/HOI-oversimplified/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/hoi.dir/src/init.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/hoi.dir/src/init.cpp.o -MF CMakeFiles/hoi.dir/src/init.cpp.o.d -o CMakeFiles/hoi.dir/src/init.cpp.o -c /home/pia/Documents/HOI-oversimplified/src/init.cpp

CMakeFiles/hoi.dir/src/init.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/hoi.dir/src/init.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pia/Documents/HOI-oversimplified/src/init.cpp > CMakeFiles/hoi.dir/src/init.cpp.i

CMakeFiles/hoi.dir/src/init.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/hoi.dir/src/init.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pia/Documents/HOI-oversimplified/src/init.cpp -o CMakeFiles/hoi.dir/src/init.cpp.s

CMakeFiles/hoi.dir/src/interface.cpp.o: CMakeFiles/hoi.dir/flags.make
CMakeFiles/hoi.dir/src/interface.cpp.o: /home/pia/Documents/HOI-oversimplified/src/interface.cpp
CMakeFiles/hoi.dir/src/interface.cpp.o: CMakeFiles/hoi.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/pia/Documents/HOI-oversimplified/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/hoi.dir/src/interface.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/hoi.dir/src/interface.cpp.o -MF CMakeFiles/hoi.dir/src/interface.cpp.o.d -o CMakeFiles/hoi.dir/src/interface.cpp.o -c /home/pia/Documents/HOI-oversimplified/src/interface.cpp

CMakeFiles/hoi.dir/src/interface.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/hoi.dir/src/interface.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pia/Documents/HOI-oversimplified/src/interface.cpp > CMakeFiles/hoi.dir/src/interface.cpp.i

CMakeFiles/hoi.dir/src/interface.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/hoi.dir/src/interface.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pia/Documents/HOI-oversimplified/src/interface.cpp -o CMakeFiles/hoi.dir/src/interface.cpp.s

CMakeFiles/hoi.dir/src/countries.cpp.o: CMakeFiles/hoi.dir/flags.make
CMakeFiles/hoi.dir/src/countries.cpp.o: /home/pia/Documents/HOI-oversimplified/src/countries.cpp
CMakeFiles/hoi.dir/src/countries.cpp.o: CMakeFiles/hoi.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/pia/Documents/HOI-oversimplified/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/hoi.dir/src/countries.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/hoi.dir/src/countries.cpp.o -MF CMakeFiles/hoi.dir/src/countries.cpp.o.d -o CMakeFiles/hoi.dir/src/countries.cpp.o -c /home/pia/Documents/HOI-oversimplified/src/countries.cpp

CMakeFiles/hoi.dir/src/countries.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/hoi.dir/src/countries.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pia/Documents/HOI-oversimplified/src/countries.cpp > CMakeFiles/hoi.dir/src/countries.cpp.i

CMakeFiles/hoi.dir/src/countries.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/hoi.dir/src/countries.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pia/Documents/HOI-oversimplified/src/countries.cpp -o CMakeFiles/hoi.dir/src/countries.cpp.s

# Object files for target hoi
hoi_OBJECTS = \
"CMakeFiles/hoi.dir/src/main.cpp.o" \
"CMakeFiles/hoi.dir/src/init.cpp.o" \
"CMakeFiles/hoi.dir/src/interface.cpp.o" \
"CMakeFiles/hoi.dir/src/countries.cpp.o"

# External object files for target hoi
hoi_EXTERNAL_OBJECTS =

/home/pia/Documents/HOI-oversimplified/hoi: CMakeFiles/hoi.dir/src/main.cpp.o
/home/pia/Documents/HOI-oversimplified/hoi: CMakeFiles/hoi.dir/src/init.cpp.o
/home/pia/Documents/HOI-oversimplified/hoi: CMakeFiles/hoi.dir/src/interface.cpp.o
/home/pia/Documents/HOI-oversimplified/hoi: CMakeFiles/hoi.dir/src/countries.cpp.o
/home/pia/Documents/HOI-oversimplified/hoi: CMakeFiles/hoi.dir/build.make
/home/pia/Documents/HOI-oversimplified/hoi: /usr/lib/x86_64-linux-gnu/libsfml-graphics.so.2.5.1
/home/pia/Documents/HOI-oversimplified/hoi: /usr/lib/x86_64-linux-gnu/libsfml-window.so.2.5.1
/home/pia/Documents/HOI-oversimplified/hoi: /usr/lib/x86_64-linux-gnu/libsfml-system.so.2.5.1
/home/pia/Documents/HOI-oversimplified/hoi: CMakeFiles/hoi.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/pia/Documents/HOI-oversimplified/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable /home/pia/Documents/HOI-oversimplified/hoi"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hoi.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/hoi.dir/build: /home/pia/Documents/HOI-oversimplified/hoi
.PHONY : CMakeFiles/hoi.dir/build

CMakeFiles/hoi.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/hoi.dir/cmake_clean.cmake
.PHONY : CMakeFiles/hoi.dir/clean

CMakeFiles/hoi.dir/depend:
	cd /home/pia/Documents/HOI-oversimplified/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pia/Documents/HOI-oversimplified /home/pia/Documents/HOI-oversimplified /home/pia/Documents/HOI-oversimplified/build /home/pia/Documents/HOI-oversimplified/build /home/pia/Documents/HOI-oversimplified/build/CMakeFiles/hoi.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/hoi.dir/depend

