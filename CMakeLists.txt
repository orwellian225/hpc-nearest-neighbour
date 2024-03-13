cmake_minimum_required(VERSION 3.16)

project(HPC-KNN DESCRIPTION "COMS4040A HPC Assignment 1: K Nearest Neigbour" LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

Include(FetchContent)

FetchContent_Declare(
    catch 
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v2.13.9
)

FetchContent_MakeAvailable(catch)

add_executable(knn src/main.cpp src/VecN.cpp src/auxillary.cpp src/serial.cpp)
target_include_directories(knn PUBLIC include)
set_target_properties(knn PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/)
add_library(Catch2::Catch IMPORTED INTERFACE)
add_executable(knn_tests tests/knn_tests.cpp src/VecN.cpp src/auxillary.cpp src/serial.cpp)
target_include_directories(knn_tests PUBLIC include)