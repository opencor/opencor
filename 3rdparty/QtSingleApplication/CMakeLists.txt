PROJECT(QtSingleApplication)

# Some in-house CMake functions/macros

INCLUDE(../../cmake/common.cmake)

# Initialise the third-party library

INITIALISE_THIRD_PARTY_LIBRARY()

# Files that make up the third-party library

SET(SOURCES
    src/qtlocalpeer.cpp
    src/qtsingleapplication.cpp
)

SET(HEADERS_MOC
    src/qtlocalpeer.h
    src/qtsingleapplication.h
)

# Various include directories

SET(LOCAL_THIRD_PARTY_LIBRARY_INCLUDE_DIRS src)
SET(THIRD_PARTY_LIBRARY_INCLUDE_DIRS ${LOCAL_THIRD_PARTY_LIBRARY_INCLUDE_DIRS} PARENT_SCOPE)

INCLUDE_DIRECTORIES(${LOCAL_THIRD_PARTY_LIBRARY_INCLUDE_DIRS})

# Build the third-party library

BUILD_THIRD_PARTY_LIBRARY(${PROJECT_NAME}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
)

# Package the third-party library

PACKAGE_THIRD_PARTY_LIBRARY()
