cmake_minimum_required(VERSION 3.16)

project(HPC-KNN DESCRIPTION "COMS4040A HPC Assignment 1: K Nearest Neigbour" LANGUAGES CXX)

add_executable(knn src/main.cpp src/VecN.cpp)
target_include_directories(knn PUBLIC include)
set_target_properties(knn PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
