cmake_minimum_required(VERSION 3.31) 
project(OSDungeon LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory("logic")
add_subdirectory("editor")