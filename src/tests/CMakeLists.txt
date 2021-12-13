project(parallel_qsort_tests)

cmake_minimum_required(VERSION 3.10)

add_subdirectory(./googletest)

#include_directories(../parallel_tools)

add_executable(run_tests
        run_primitives_tests.cpp
        sequentional_tools.h
        )
target_link_libraries(run_tests pthread gtest gtest_main)