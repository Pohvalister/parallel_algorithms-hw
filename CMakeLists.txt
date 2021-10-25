cmake_minimum_required(VERSION 3.16)
project(parallel_qsort)

set(CMAKE_CXX_STANDARD 17)
link_directories(./lib)
include_directories(./include)

include_directories(./src)

add_compile_options(-fcilkplus -std=c++17)

add_executable(parallel_qsort
        src/quicksorts.h
        src/parallel_tools/parallel_map.h
        main.cpp)##
target_link_libraries(parallel_qsort pthread)##cilkrts

add_subdirectory(src/tests)

