cmake_minimum_required(VERSION 3.11)
project(skiplist)

set(PROJECT_NAME skiplist)

set(CMAKE_CXX_STANDARD 14)

add_executable(main main.cpp skiplist.h node.h)

add_subdirectory(test)
