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

# Utility rule file for test_fanotify_autogen.

# Include the progress variables for this target.
include CMakeFiles/test_fanotify_autogen.dir/progress.make

CMakeFiles/test_fanotify_autogen:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC for target test_fanotify"
	/usr/bin/cmake -E cmake_autogen /home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/build/CMakeFiles/test_fanotify_autogen.dir/AutogenInfo.cmake ""

test_fanotify_autogen: CMakeFiles/test_fanotify_autogen
test_fanotify_autogen: CMakeFiles/test_fanotify_autogen.dir/build.make

.PHONY : test_fanotify_autogen

# Rule to build all files generated by this target.
CMakeFiles/test_fanotify_autogen.dir/build: test_fanotify_autogen

.PHONY : CMakeFiles/test_fanotify_autogen.dir/build

CMakeFiles/test_fanotify_autogen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_fanotify_autogen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_fanotify_autogen.dir/clean

CMakeFiles/test_fanotify_autogen.dir/depend:
	cd /home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify /home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify /home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/build /home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/build /home/zwh/linux_cems/src_en/dbac/notify_demo/test_fanotify/build/CMakeFiles/test_fanotify_autogen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_fanotify_autogen.dir/depend
