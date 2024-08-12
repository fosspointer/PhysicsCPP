cmake_minimum_required(VERSION 3.16)

project(physics VERSION 0.2 DESCRIPTION "Tool for demonstrating 10th greek grade physics in C++/SFML" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

add_subdirectory(ext)
add_subdirectory(core)
add_subdirectory(demo)

option(generate_docs "Use doxygen to generate documentation" ON)

set(DOXYGEN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/docs)

if(generate_docs)
    find_package(Doxygen)

    if(DOXYGEN_FOUND)
        set(DOXYGEN_IN ${DOXYGEN_DIR}/Doxyfile.in)
        set(DOXYGEN_OUT ${DOXYGEN_DIR}/Doxyfile.out)

        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        message("Attempting to build documentation")

        add_custom_target(docs
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${DOXYGEN_DIR}
            COMMENT "Started generating documentation")
    else()
        message("Doxygen is not installed on your machine or could not be located")
    endif()
endif()