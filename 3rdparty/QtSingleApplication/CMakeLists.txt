PROJECT(QtSingleApplication)

# Some in-house CMake functions/macros

INCLUDE(../../cmake/common.cmake)

# Initialise QtSingleApplication

INITIALISE_THIRD_PARTY_LIBRARY()

# Files that make up QtSingleApplication

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

# Rules to build QtSingleApplication

BUILD_THIRD_PARTY_LIBRARY(${PROJECT_NAME}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
)

# Rules to package QtSingleApplication

PACKAGE_THIRD_PARTY_LIBRARY()
