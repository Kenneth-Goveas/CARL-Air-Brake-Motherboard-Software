cmake_minimum_required(VERSION 3.20.0)
project(CARL-Air-Brake-Motherboard-Software LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -Wpedantic")

add_subdirectory(lib)
add_subdirectory(app)
