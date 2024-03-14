cmake_minimum_required(VERSION 3.16)

project(HPC-KNN DESCRIPTION "COMS4040A HPC Assignment 1: K Nearest Neigbour" LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

Include(FetchContent)

find_package(OpenMP REQUIRED)

add_executable(knn src/main.cpp src/VecN.cpp src/auxillary.cpp src/serial.cpp src/omp_sections.cpp src/omp_tasks.cpp)
target_include_directories(knn PUBLIC include)
set_target_properties(knn PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
target_link_libraries(knn PRIVATE OpenMP::OpenMP_CXX)

set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/)
add_library(Catch2::Catch IMPORTED INTERFACE)
add_executable(knn_tests tests/knn_tests.cpp src/VecN.cpp src/auxillary.cpp src/serial.cpp src/omp_sections.cpp src/omp_tasks.cpp)
target_include_directories(knn_tests PUBLIC include)
target_link_libraries(knn_tests PRIVATE OpenMP::OpenMP_CXX)
