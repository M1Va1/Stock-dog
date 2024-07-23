cmake_minimum_required(VERSION 3.10)

project(Stockdog)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    src/basic_classes.cpp
    src/move_generation.cpp
    src/debug.cpp
)

add_executable(Stockdog ${SOURCES})


target_include_directories(Stockdog PRIVATE ${CMAKE_SOURCE_DIR}/src)
