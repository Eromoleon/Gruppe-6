# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.8

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "D:\Programok81\CLion 2017.2.3\bin\cmake\bin\cmake.exe"

# The command to remove a file.
RM = "D:\Programok81\CLion 2017.2.3\bin\cmake\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\Dokumentumok\MscI_offline\Clion__PROJECTS\Test4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\Dokumentumok\MscI_offline\Clion__PROJECTS\Test4\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Test4.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Test4.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Test4.dir/flags.make

CMakeFiles/Test4.dir/main.cpp.obj: CMakeFiles/Test4.dir/flags.make
CMakeFiles/Test4.dir/main.cpp.obj: CMakeFiles/Test4.dir/includes_CXX.rsp
CMakeFiles/Test4.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Dokumentumok\MscI_offline\Clion__PROJECTS\Test4\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Test4.dir/main.cpp.obj"
	C:\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Test4.dir\main.cpp.obj -c D:\Dokumentumok\MscI_offline\Clion__PROJECTS\Test4\main.cpp

CMakeFiles/Test4.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Test4.dir/main.cpp.i"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Dokumentumok\MscI_offline\Clion__PROJECTS\Test4\main.cpp > CMakeFiles\Test4.dir\main.cpp.i

CMakeFiles/Test4.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Test4.dir/main.cpp.s"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Dokumentumok\MscI_offline\Clion__PROJECTS\Test4\main.cpp -o CMakeFiles\Test4.dir\main.cpp.s

CMakeFiles/Test4.dir/main.cpp.obj.requires:

.PHONY : CMakeFiles/Test4.dir/main.cpp.obj.requires

CMakeFiles/Test4.dir/main.cpp.obj.provides: CMakeFiles/Test4.dir/main.cpp.obj.requires
	$(MAKE) -f CMakeFiles\Test4.dir\build.make CMakeFiles/Test4.dir/main.cpp.obj.provides.build
.PHONY : CMakeFiles/Test4.dir/main.cpp.obj.provides

CMakeFiles/Test4.dir/main.cpp.obj.provides.build: CMakeFiles/Test4.dir/main.cpp.obj


# Object files for target Test4
Test4_OBJECTS = \
"CMakeFiles/Test4.dir/main.cpp.obj"

# External object files for target Test4
Test4_EXTERNAL_OBJECTS =

Test4.exe: CMakeFiles/Test4.dir/main.cpp.obj
Test4.exe: CMakeFiles/Test4.dir/build.make
Test4.exe: C:/UOCV/build/lib/libopencv_ml331.dll.a
Test4.exe: C:/UOCV/build/lib/libopencv_objdetect331.dll.a
Test4.exe: C:/UOCV/build/lib/libopencv_shape331.dll.a
Test4.exe: C:/UOCV/build/lib/libopencv_stitching331.dll.a
Test4.exe: C:/UOCV/build/lib/libopencv_superres331.dll.a
Test4.exe: C:/UOCV/build/lib/libopencv_videostab331.dll.a
Test4.exe: C:/UOCV/build/lib/libopencv_calib3d331.dll.a
Test4.exe: C:/UOCV/build/lib/libopencv_features2d331.dll.a
Test4.exe: C:/UOCV/build/lib/libopencv_flann331.dll.a
Test4.exe: C:/UOCV/build/lib/libopencv_highgui331.dll.a
Test4.exe: C:/UOCV/build/lib/libopencv_photo331.dll.a
Test4.exe: C:/UOCV/build/lib/libopencv_video331.dll.a
Test4.exe: C:/UOCV/build/lib/libopencv_videoio331.dll.a
Test4.exe: C:/UOCV/build/lib/libopencv_imgcodecs331.dll.a
Test4.exe: C:/UOCV/build/lib/libopencv_imgproc331.dll.a
Test4.exe: C:/UOCV/build/lib/libopencv_core331.dll.a
Test4.exe: CMakeFiles/Test4.dir/linklibs.rsp
Test4.exe: CMakeFiles/Test4.dir/objects1.rsp
Test4.exe: CMakeFiles/Test4.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\Dokumentumok\MscI_offline\Clion__PROJECTS\Test4\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Test4.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Test4.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Test4.dir/build: Test4.exe

.PHONY : CMakeFiles/Test4.dir/build

CMakeFiles/Test4.dir/requires: CMakeFiles/Test4.dir/main.cpp.obj.requires

.PHONY : CMakeFiles/Test4.dir/requires

CMakeFiles/Test4.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Test4.dir\cmake_clean.cmake
.PHONY : CMakeFiles/Test4.dir/clean

CMakeFiles/Test4.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Dokumentumok\MscI_offline\Clion__PROJECTS\Test4 D:\Dokumentumok\MscI_offline\Clion__PROJECTS\Test4 D:\Dokumentumok\MscI_offline\Clion__PROJECTS\Test4\cmake-build-debug D:\Dokumentumok\MscI_offline\Clion__PROJECTS\Test4\cmake-build-debug D:\Dokumentumok\MscI_offline\Clion__PROJECTS\Test4\cmake-build-debug\CMakeFiles\Test4.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Test4.dir/depend
