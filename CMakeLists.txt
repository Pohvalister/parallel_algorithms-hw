cmake_minimum_required(VERSION 3.10)
project(parallel_qsort)

set(CMAKE_CXX_STANDARD 17)
link_directories(./cilk/lib)
include_directories(./cilk/include)

include_directories(./src)

add_compile_options(-std=c++17)##-fcilkplus -lcilkrts <- useless?

add_executable(parallel_qsort
        src/quicksorts.h
        src/parallel_tools/.
        main.cpp)##
target_link_libraries(parallel_qsort pthread)##

add_subdirectory(src/tests)
