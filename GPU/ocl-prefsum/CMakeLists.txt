cmake_minimum_required(VERSION 3.21)
project(ocl1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23")

find_package(OpenCL REQUIRED)
add_executable(ocl2 main.cpp)
target_link_libraries(ocl2 OpenCL::OpenCL)