cmake_minimum_required(VERSION 3.8)
project(straighten)

set(CMAKE_CXX_STANDARD 11)
set(OpenCV_DIR "C:\\UOCV\\build")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


set(SOURCE_FILES straighten.cpp)
add_executable(straighten ${SOURCE_FILES})

target_link_libraries( straighten ${OpenCV_LIBS} )


set(SOURCE_FILES straighten.cpp.cpp)


