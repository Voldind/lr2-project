cmake_minimum_required(VERSION 3.16)
project(DataStructures_Lab2 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Общая библиотека с алгоритмами
add_library(lab2_core
    src/arithmetic_eval.cpp
    src/partition.cpp
    src/powerset.cpp
    src/right_view.cpp
    src/hash_functions.cpp
    src/file_utils.cpp
)

# Исполняемые файлы для каждого задания
add_executable(task1_stack apps/task1_stack.cpp)
target_link_libraries(task1_stack PRIVATE lab2_core)

add_executable(task2_set_cli apps/task2_set_cli.cpp)
target_link_libraries(task2_set_cli PRIVATE lab2_core)

add_executable(task3_partition apps/task3_partition.cpp)
target_link_libraries(task3_partition PRIVATE lab2_core)

add_executable(task4_subarrays apps/task4_subarrays.cpp)
target_link_libraries(task4_subarrays PRIVATE lab2_core)

add_executable(task5_tree_view apps/task5_tree_view.cpp)
target_link_libraries(task5_tree_view PRIVATE lab2_core)

add_executable(task6_hashtable apps/task6_hashtable.cpp)
target_link_libraries(task6_hashtable PRIVATE lab2_core)

add_executable(task7_lru apps/task7_lru.cpp)
target_link_libraries(task7_lru PRIVATE lab2_core)