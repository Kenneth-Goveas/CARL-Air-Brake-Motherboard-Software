cmake_minimum_required(VERSION 3.20.0)
project(CARL-Air-Brake-Motherboard-Software LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

add_subdirectory(lib)
add_subdirectory(unit)
