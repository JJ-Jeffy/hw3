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
CMAKE_COMMAND = /global/common/software/nersc/pm-2021q4/sw/cmake-3.22.0/bin/cmake

# The command to remove a file.
RM = /global/common/software/nersc/pm-2021q4/sw/cmake-3.22.0/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /global/homes/j/jj-jeffy/Berkeley267/hw3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /global/homes/j/jj-jeffy/Berkeley267/hw3/build

# Include any dependencies generated for this target.
include CMakeFiles/kmer_hash_19.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/kmer_hash_19.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/kmer_hash_19.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/kmer_hash_19.dir/flags.make

CMakeFiles/kmer_hash_19.dir/kmer_hash.cpp.o: CMakeFiles/kmer_hash_19.dir/flags.make
CMakeFiles/kmer_hash_19.dir/kmer_hash.cpp.o: ../kmer_hash.cpp
CMakeFiles/kmer_hash_19.dir/kmer_hash.cpp.o: CMakeFiles/kmer_hash_19.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/global/homes/j/jj-jeffy/Berkeley267/hw3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/kmer_hash_19.dir/kmer_hash.cpp.o"
	/opt/cray/pe/craype/2.7.30/bin/CC $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/kmer_hash_19.dir/kmer_hash.cpp.o -MF CMakeFiles/kmer_hash_19.dir/kmer_hash.cpp.o.d -o CMakeFiles/kmer_hash_19.dir/kmer_hash.cpp.o -c /global/homes/j/jj-jeffy/Berkeley267/hw3/kmer_hash.cpp

CMakeFiles/kmer_hash_19.dir/kmer_hash.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kmer_hash_19.dir/kmer_hash.cpp.i"
	/opt/cray/pe/craype/2.7.30/bin/CC $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /global/homes/j/jj-jeffy/Berkeley267/hw3/kmer_hash.cpp > CMakeFiles/kmer_hash_19.dir/kmer_hash.cpp.i

CMakeFiles/kmer_hash_19.dir/kmer_hash.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kmer_hash_19.dir/kmer_hash.cpp.s"
	/opt/cray/pe/craype/2.7.30/bin/CC $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /global/homes/j/jj-jeffy/Berkeley267/hw3/kmer_hash.cpp -o CMakeFiles/kmer_hash_19.dir/kmer_hash.cpp.s

# Object files for target kmer_hash_19
kmer_hash_19_OBJECTS = \
"CMakeFiles/kmer_hash_19.dir/kmer_hash.cpp.o"

# External object files for target kmer_hash_19
kmer_hash_19_EXTERNAL_OBJECTS =

kmer_hash_19: CMakeFiles/kmer_hash_19.dir/kmer_hash.cpp.o
kmer_hash_19: CMakeFiles/kmer_hash_19.dir/build.make
kmer_hash_19: CMakeFiles/kmer_hash_19.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/global/homes/j/jj-jeffy/Berkeley267/hw3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable kmer_hash_19"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/kmer_hash_19.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/kmer_hash_19.dir/build: kmer_hash_19
.PHONY : CMakeFiles/kmer_hash_19.dir/build

CMakeFiles/kmer_hash_19.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/kmer_hash_19.dir/cmake_clean.cmake
.PHONY : CMakeFiles/kmer_hash_19.dir/clean

CMakeFiles/kmer_hash_19.dir/depend:
	cd /global/homes/j/jj-jeffy/Berkeley267/hw3/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /global/homes/j/jj-jeffy/Berkeley267/hw3 /global/homes/j/jj-jeffy/Berkeley267/hw3 /global/homes/j/jj-jeffy/Berkeley267/hw3/build /global/homes/j/jj-jeffy/Berkeley267/hw3/build /global/homes/j/jj-jeffy/Berkeley267/hw3/build/CMakeFiles/kmer_hash_19.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/kmer_hash_19.dir/depend
