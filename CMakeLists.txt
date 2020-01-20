cmake_minimum_required(VERSION 3.15)
project(CS764_lichengxi_huang)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall")

add_subdirectory(third_party)
include_directories(third_party)

add_subdirectory(src)