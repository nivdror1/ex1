cmake_minimum_required(VERSION 3.6)
project(ex1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES simpletest.cpp osm.cpp)
add_executable(ex1 ${SOURCE_FILES})