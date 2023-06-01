cmake_minimum_required(VERSION 3.16)

project(physics VERSION 0.1 DESCRIPTION "Tool for demonstrating 10th greek grade physics in C++/SFML" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

find_package(Doxygen REQUIRED)

add_subdirectory(ext)
add_subdirectory(core)
add_subdirectory(demo)

doxygen_add_docs(documentation
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Physics.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Physics/*/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.hpp
    )