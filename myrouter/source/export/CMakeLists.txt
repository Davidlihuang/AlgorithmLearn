cmake_minimum_required(VERSION 3.20.0)
#set(projectName, "pcbRouter")
project(pcbRouter)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)
space_in_quoted_tokens
set(hpp dsnparser.h mymath.h )
set(source main.cpp dsnparser.cpp mymath.cpp )

add_executable(${PROJECT_NAME} ${source})
target_include_directories(${PROJECT_NAME}  PUBLIC  .)
#target_link_libraries(${PROJECT_NAME} -lpthread)