cmake_minimum_required(VERSION 3.21)
project(oop_alexandrov)

set(CMAKE_CXX_STANDARD 14)
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

add_executable(oop_alexandrov
        main.cpp Container.cpp Container.h Object.cpp Object.h)
