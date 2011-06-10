PROJECT(QtMmlWidget)

# Some in-house CMake functions/macros

INCLUDE(../../cmake/common.cmake)

# Initialise the third-party library

INITIALISE_THIRD_PARTY_LIBRARY()

# Files that make up the third-party library

SET(SOURCES
    src/qtmmlwidget.cpp
)

SET(HEADERS
    src/qtmmlwidget.h
)

# Various include directories

SET(LOCAL_THIRD_PARTY_LIBRARY_INCLUDE_DIRS include src)
SET(THIRD_PARTY_LIBRARY_INCLUDE_DIRS ${LOCAL_THIRD_PARTY_LIBRARY_INCLUDE_DIRS} PARENT_SCOPE)

INCLUDE_DIRECTORIES(${LOCAL_THIRD_PARTY_LIBRARY_INCLUDE_DIRS})

# Build the third-party library

BUILD_THIRD_PARTY_LIBRARY(
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTXML_LIBRARY}
)

# Package the third-party library

PACKAGE_THIRD_PARTY_LIBRARY()
