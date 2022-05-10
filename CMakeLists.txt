cmake_minimum_required(VERSION 3.5)
project(opencv)

find_package(OpenCV REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp)
add_executable(opencv ${SOURCE_FILES})

find_package(MPI)
include_directories(${MPI_INCLUDE_PATH})


include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(opencv ${OpenCV_LIBS} MPI::MPI_CXX)