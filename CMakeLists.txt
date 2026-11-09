cmake_minimum_required(VERSION 3.16)
project(DataStructures_Lab2 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(include)

add_executable(task1_stack apps/task1_stack.cpp)
add_executable(task2_set_cli apps/task2_set_cli.cpp)
add_executable(task3_partition apps/task3_partition.cpp)
add_executable(task4_subarrays apps/task4_subarrays.cpp)
add_executable(task5_tree_view apps/task5_tree_view.cpp)
add_executable(task6_hashtable apps/task6_hashtable.cpp)
add_executable(task7_lru apps/task7_lru.cpp)