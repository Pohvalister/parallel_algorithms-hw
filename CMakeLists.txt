cmake_minimum_required(VERSION 3.10)
project(parallel_qsort)
set(CMAKE_CXX_STANDARD 17)

link_directories(./cilk/lib)
include_directories(./cilk/include)

include_directories(./src)

add_compile_options(-g -DUSE_CILK_PLUS_RUNTIME -std=c++17)

add_executable(parallel_qsort
        src/parallel_quicksort.h
        src/parallel_tools/.
        main.cpp)##
target_link_libraries(parallel_qsort pthread)##\

add_subdirectory(src/tests)
