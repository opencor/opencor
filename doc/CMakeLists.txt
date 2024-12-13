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

find_package(Python 3.12 EXACT COMPONENTS Interpreter)

find_program(SPHINX_EXECUTABLE NAMES sphinx-build sphinx-build2)

check_python_package(sphinx-copybutton PYTHON_SPHINX_COPY_BUTTON_AVAILABLE)
check_python_package(sphinx-inline-tabs PYTHON_SPHINX_INLINE_TABS_AVAILABLE)

if(SPHINX_EXECUTABLE AND PYTHON_SPHINX_COPY_BUTTON_AVAILABLE AND PYTHON_SPHINX_INLINE_TABS_AVAILABLE)
    generate_documentation(FALSE)
else()
    message(FATAL_ERROR "No documentation could be generated...")
endif()
