cmake_minimum_required(VERSION 3.21)
project(socow-vector)

set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)

file(GLOB TEST_SRC test/*.cpp)
add_executable(tests ${TEST_SRC})

target_include_directories(tests PRIVATE src test)

if(MSVC)
  target_compile_options(tests PRIVATE /W4 /permissive-)
  if(TREAT_WARNINGS_AS_ERRORS)
    target_compile_options(tests PRIVATE /WX)
  endif()
else()
  target_compile_options(tests PRIVATE -Wall -Wextra -Wno-sign-compare)
  target_compile_options(tests PRIVATE -Wold-style-cast -Wextra-semi -Woverloaded-virtual -Wzero-as-null-pointer-constant)
  target_compile_options(tests PRIVATE -Wpointer-arith -Wvla)
  if(TREAT_WARNINGS_AS_ERRORS)
    target_compile_options(tests PRIVATE -Werror)
  endif()
endif()

# Compiler specific warnings
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(tests PRIVATE -Wshadow=compatible-local)
  target_compile_options(tests PRIVATE -Wduplicated-branches)
  target_compile_options(tests PRIVATE -Wduplicated-cond)
  target_compile_options(tests PRIVATE -Wnull-dereference)
  target_compile_options(tests PRIVATE -Walloc-zero)
  # False positives
  target_compile_options(tests PRIVATE -Wno-array-bounds)
  target_compile_options(tests PRIVATE -Wno-maybe-uninitialized)
  target_compile_options(tests PRIVATE -Wno-stringop-overflow -Wno-stringop-overread)
  target_compile_options(tests PRIVATE -Wno-use-after-free)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_compile_options(tests PRIVATE -Wshadow-uncaptured-local)
  target_compile_options(tests PRIVATE -Wloop-analysis)
  target_compile_options(tests PRIVATE -Wno-self-assign-overloaded)
  target_compile_options(tests PRIVATE -Wpedantic -Wno-flexible-array-extensions -Wno-zero-length-array)
endif()

option(USE_SANITIZERS "Enable to build with undefined,leak and address sanitizers" OFF)
if(USE_SANITIZERS)
  message(STATUS "Enabling sanitizers...")
  target_compile_options(tests PUBLIC -fsanitize=address,undefined,leak -fno-sanitize-recover=all)
  target_link_options(tests PUBLIC -fsanitize=address,undefined,leak)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  message(STATUS "Enabling libc++...")
  target_compile_options(tests PUBLIC -stdlib=libc++)
  target_link_options(tests PUBLIC -stdlib=libc++)
endif()

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  message(STATUS "Enabling _GLIBCXX_DEBUG...")
  target_compile_options(tests PUBLIC -D_GLIBCXX_DEBUG)
endif()

target_link_libraries(tests GTest::gtest GTest::gtest_main)
