cmake_minimum_rquired(VERSION 3.16)

project(HPC-Nearest-Neighbour DESCRIPTION "COMS4040A HPC Assignment 1: K Nearest Neigbour" LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(lib/fmt)

add_executable(knn
    src/main.cpp
    src/serial.cpp src/tasks.cpp src/sections.cpp
)

target_include_directories(knn PUBLIC include PUBLIC lib/fmt/include)
target_link_directories(knn PRIVATE src PRIVATE lib/fmt/src)
target_link_libraries(knn fmt)

set_target_properties(knn PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
