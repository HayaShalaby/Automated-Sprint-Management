cmake_minimum_required(VERSION 3.26)
project(sprinter_)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(sprinter_
        sprint.cpp
        sprint.h
        main.cpp)
