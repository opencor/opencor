cmake_minimum_required(VERSION 3.14)

# Project definition

project(OpenCOR
    LANGUAGES NONE
)

# Some in-house CMake macros

set(CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/..)

include(${CMAKE_SOURCE_DIR}/cmake/common.cmake)

# Configure external projects

set(PROJECT_BUILD_DIR ${CMAKE_BINARY_DIR})

configure_external_projects()

# Generate our documentation

generate_documentation(FALSE)
