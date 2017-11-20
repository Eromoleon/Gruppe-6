cmake_minimum_required(VERSION 3.8)
project(Test4)

set(CMAKE_CXX_STANDARD 11)
set(OpenCV_DIR "C:\\UOCV\\build")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


set(SOURCE_FILES main.cpp)
add_executable(Test4 ${SOURCE_FILES})

target_link_libraries( Test4 ${OpenCV_LIBS} )


set(SOURCE_FILES main.cpp)




