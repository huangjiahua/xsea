cmake_minimum_required(VERSION 3.10)
project(xsea)

set(CMAKE_CXX_STANDARD 11)

add_executable(xsea main.cpp include/xsea.h src/document.cpp src/node.cpp)