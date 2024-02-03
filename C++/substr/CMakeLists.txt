cmake_minimum_required(VERSION 3.17)
project(substr_task)

set(CMAKE_CXX_STANDARD 20)

add_executable(solution solution.cpp)

if (MSVC)
    target_compile_options(solution PRIVATE /W4 /permissive-)
    if (TREAT_WARNINGS_AS_ERRORS)
        target_compile_options(solution PRIVATE /WX)
    endif()
else()
    target_compile_options(solution PRIVATE -Wall -Wextra -pedantic -Wno-sign-compare)
    if (TREAT_WARNINGS_AS_ERRORS)
        target_compile_options(solution PRIVATE -Werror)
    endif()
endif()

# Compiler specific warnings
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(solution PRIVATE -Wshadow=compatible-local)
    target_compile_options(solution PRIVATE -Wduplicated-branches)
    target_compile_options(solution PRIVATE -Wduplicated-cond)
    target_compile_options(solution PRIVATE -Wnull-dereference)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(solution PRIVATE -Wshadow-uncaptured-local)
    target_compile_options(solution PRIVATE -Wloop-analysis)
endif()
