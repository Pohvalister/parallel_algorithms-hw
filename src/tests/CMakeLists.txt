project(parallel_qsort_tests)

cmake_minimum_required(VERSION 3.10)

add_subdirectory(./googletest)

add_compile_options(-Rpass-analysis=loop-spawning-ti)

add_executable(run_primitives_tests
        run_primitives_tests.cpp
        )

add_executable(run_qsort_tests
        run_qsort_tests.cpp
        )
target_link_libraries(run_primitives_tests pthread cilkrts gtest gtest_main)
target_link_libraries(run_qsort_tests pthread cilkrts gtest gtest_main)