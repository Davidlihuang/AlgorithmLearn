cmake_minimum_required(VERSION 3.20.0)
set(projectName "Dijkstra")
project(${projectName})

#设置编译选项
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_STANDARD 11) #c++标准
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${projectName} dijkstra.cpp dijkstra.h)