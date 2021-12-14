cmake_minimum_required(VERSION 3.10)
project(parallel_qsort)
set(CMAKE_CXX_STANDARD 17)

link_directories(./cilk/lib)
include_directories(./cilk/include)

include_directories(./parallel_qsort)

add_compile_options(-g -fcilkplus -DUSE_CILK_PLUS_RUNTIME -std=c++17)

add_executable(parallel_qsort
        parallel_qsort/parallel_quicksort.h
        parallel_qsort/parallel_tools/.
        main.cpp)
target_link_libraries(parallel_qsort pthread cilkrts)

add_subdirectory(parallel_qsort/tests)
