cmake_minimum_required(VERSION 3.10)
project(xsea CXX)

set(CMAKE_CXX_STANDARD 11)

set(LIB_INCLUDE include/xsea.h)
set(LIB_SOURCE src/document.cpp src/node.cpp src/nonelement.cpp src/element.cpp)


add_library(xsea SHARED ${LIB_SOURCE})
add_subdirectory(./sample)

