# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/build

# Include any dependencies generated for this target.
include CMakeFiles/test_fanotify.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test_fanotify.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_fanotify.dir/flags.make

CMakeFiles/test_fanotify.dir/filelisten/common.cpp.o: CMakeFiles/test_fanotify.dir/flags.make
CMakeFiles/test_fanotify.dir/filelisten/common.cpp.o: ../filelisten/common.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_fanotify.dir/filelisten/common.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_fanotify.dir/filelisten/common.cpp.o -c /home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/filelisten/common.cpp

CMakeFiles/test_fanotify.dir/filelisten/common.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_fanotify.dir/filelisten/common.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/filelisten/common.cpp > CMakeFiles/test_fanotify.dir/filelisten/common.cpp.i

CMakeFiles/test_fanotify.dir/filelisten/common.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_fanotify.dir/filelisten/common.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/filelisten/common.cpp -o CMakeFiles/test_fanotify.dir/filelisten/common.cpp.s

CMakeFiles/test_fanotify.dir/filelisten/fanotify_file_listener.cpp.o: CMakeFiles/test_fanotify.dir/flags.make
CMakeFiles/test_fanotify.dir/filelisten/fanotify_file_listener.cpp.o: ../filelisten/fanotify_file_listener.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/test_fanotify.dir/filelisten/fanotify_file_listener.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_fanotify.dir/filelisten/fanotify_file_listener.cpp.o -c /home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/filelisten/fanotify_file_listener.cpp

CMakeFiles/test_fanotify.dir/filelisten/fanotify_file_listener.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_fanotify.dir/filelisten/fanotify_file_listener.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/filelisten/fanotify_file_listener.cpp > CMakeFiles/test_fanotify.dir/filelisten/fanotify_file_listener.cpp.i

CMakeFiles/test_fanotify.dir/filelisten/fanotify_file_listener.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_fanotify.dir/filelisten/fanotify_file_listener.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/filelisten/fanotify_file_listener.cpp -o CMakeFiles/test_fanotify.dir/filelisten/fanotify_file_listener.cpp.s

CMakeFiles/test_fanotify.dir/filelisten/notify_manage.cpp.o: CMakeFiles/test_fanotify.dir/flags.make
CMakeFiles/test_fanotify.dir/filelisten/notify_manage.cpp.o: ../filelisten/notify_manage.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/test_fanotify.dir/filelisten/notify_manage.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_fanotify.dir/filelisten/notify_manage.cpp.o -c /home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/filelisten/notify_manage.cpp

CMakeFiles/test_fanotify.dir/filelisten/notify_manage.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_fanotify.dir/filelisten/notify_manage.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/filelisten/notify_manage.cpp > CMakeFiles/test_fanotify.dir/filelisten/notify_manage.cpp.i

CMakeFiles/test_fanotify.dir/filelisten/notify_manage.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_fanotify.dir/filelisten/notify_manage.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/filelisten/notify_manage.cpp -o CMakeFiles/test_fanotify.dir/filelisten/notify_manage.cpp.s

CMakeFiles/test_fanotify.dir/main.cpp.o: CMakeFiles/test_fanotify.dir/flags.make
CMakeFiles/test_fanotify.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/test_fanotify.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_fanotify.dir/main.cpp.o -c /home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/main.cpp

CMakeFiles/test_fanotify.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_fanotify.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/main.cpp > CMakeFiles/test_fanotify.dir/main.cpp.i

CMakeFiles/test_fanotify.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_fanotify.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/main.cpp -o CMakeFiles/test_fanotify.dir/main.cpp.s

CMakeFiles/test_fanotify.dir/test_fanotify_autogen/mocs_compilation.cpp.o: CMakeFiles/test_fanotify.dir/flags.make
CMakeFiles/test_fanotify.dir/test_fanotify_autogen/mocs_compilation.cpp.o: test_fanotify_autogen/mocs_compilation.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/test_fanotify.dir/test_fanotify_autogen/mocs_compilation.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_fanotify.dir/test_fanotify_autogen/mocs_compilation.cpp.o -c /home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/build/test_fanotify_autogen/mocs_compilation.cpp

CMakeFiles/test_fanotify.dir/test_fanotify_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_fanotify.dir/test_fanotify_autogen/mocs_compilation.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/build/test_fanotify_autogen/mocs_compilation.cpp > CMakeFiles/test_fanotify.dir/test_fanotify_autogen/mocs_compilation.cpp.i

CMakeFiles/test_fanotify.dir/test_fanotify_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_fanotify.dir/test_fanotify_autogen/mocs_compilation.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/build/test_fanotify_autogen/mocs_compilation.cpp -o CMakeFiles/test_fanotify.dir/test_fanotify_autogen/mocs_compilation.cpp.s

# Object files for target test_fanotify
test_fanotify_OBJECTS = \
"CMakeFiles/test_fanotify.dir/filelisten/common.cpp.o" \
"CMakeFiles/test_fanotify.dir/filelisten/fanotify_file_listener.cpp.o" \
"CMakeFiles/test_fanotify.dir/filelisten/notify_manage.cpp.o" \
"CMakeFiles/test_fanotify.dir/main.cpp.o" \
"CMakeFiles/test_fanotify.dir/test_fanotify_autogen/mocs_compilation.cpp.o"

# External object files for target test_fanotify
test_fanotify_EXTERNAL_OBJECTS =

test_fanotify: CMakeFiles/test_fanotify.dir/filelisten/common.cpp.o
test_fanotify: CMakeFiles/test_fanotify.dir/filelisten/fanotify_file_listener.cpp.o
test_fanotify: CMakeFiles/test_fanotify.dir/filelisten/notify_manage.cpp.o
test_fanotify: CMakeFiles/test_fanotify.dir/main.cpp.o
test_fanotify: CMakeFiles/test_fanotify.dir/test_fanotify_autogen/mocs_compilation.cpp.o
test_fanotify: CMakeFiles/test_fanotify.dir/build.make
test_fanotify: /home/zwh/Qt5.9.9/5.9.9/gcc_64/lib/libQt5Core.so.5.9.9
test_fanotify: CMakeFiles/test_fanotify.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable test_fanotify"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_fanotify.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_fanotify.dir/build: test_fanotify

.PHONY : CMakeFiles/test_fanotify.dir/build

CMakeFiles/test_fanotify.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_fanotify.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_fanotify.dir/clean

CMakeFiles/test_fanotify.dir/depend:
	cd /home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify /home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify /home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/build /home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/build /home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/build/CMakeFiles/test_fanotify.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_fanotify.dir/depend

