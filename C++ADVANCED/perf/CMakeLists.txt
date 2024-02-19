cmake_minimum_required(VERSION 3.21)
project(perf)

set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)

add_subdirectory(perf)

if(ENABLE_SLOW_TEST)
  add_subdirectory(ci-extra/perf)
endif()
