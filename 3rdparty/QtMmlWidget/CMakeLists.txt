PROJECT(QtMmlWidget)

# Some in-house CMake functions/macros

INCLUDE(../../cmake/common.cmake)

# Initialise QtMmlWidget

INITIALISE_THIRD_PARTY_LIBRARY()

# Files that make up QtMmlWidget

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

# Rules to build QtMmlWidget

BUILD_THIRD_PARTY_LIBRARY(${PROJECT_NAME}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTXML_LIBRARY}
)

# Rules to package QtMmlWidget

PACKAGE_THIRD_PARTY_LIBRARY()
