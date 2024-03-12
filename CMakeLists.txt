cmake_minimum_required(VERSION 3.16)

add_executable(knn src/main.cpp src/VecN.cpp)
target_include_directories(knn PUBLIC include)
