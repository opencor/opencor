cmake_minimum_required(VERSION 3.14)

# Project definition

project(OpenCOR
    LANGUAGES NONE
)

# Some in-house CMake macros

set(CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/..)

include(${CMAKE_SOURCE_DIR}/cmake/common.cmake)

# Add support for external projects

include(ExternalProject)

# Generate our documentation

find_package(Python COMPONENTS Interpreter)

find_program(SPHINX_EXECUTABLE NAMES sphinx-build sphinx-build2)

generate_documentation(FALSE)
