project(parallel_bfs_tests)

cmake_minimum_required(VERSION 3.10)

add_subdirectory(./googletest)
#add_compile_options(-Rpass-analysis=loop-spawning-ti)
add_compile_options(-g -fcilkplus -DUSE_CILK_PLUS_RUNTIME -std=c++17 -Rpass-analysis=loop-spawning-ti)

add_executable(run_bfs_tests
        run_bfs_tests.cpp
        )

target_link_libraries(run_bfs_tests pthread cilkrts gtest gtest_main)