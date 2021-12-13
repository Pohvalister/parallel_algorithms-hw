project(parallel_qsort_tests)

cmake_minimum_required(VERSION 3.10)

add_subdirectory(./googletest)

#include_directories(../parallel_tools)

add_executable(run_primitives_tests
        run_primitives_tests.cpp
        )

add_executable(run_qsort_tests
        qsort_tests.cpp
        )
target_link_libraries(run_primitives_tests pthread gtest gtest_main)
target_link_libraries(run_qsort_tests pthread gtest gtest_main)