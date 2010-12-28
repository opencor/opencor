PROJECT(QtSingleApplication)

# Minimum version of CMake required

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# Type of build

SET(CMAKE_BUILD_TYPE Release)

# Files that make up QtSingleApplication

SET(${PROJECT_NAME}_SOURCES
    qtlocalpeer.cpp
    qtlockedfile.cpp
    qtsingleapplication.cpp
    qtsinglecoreapplication.cpp
)

SET(${PROJECT_NAME}_HEADERS_MOC
    qtlocalpeer.h
    qtsingleapplication.h
    qtsinglecoreapplication.h
)

IF(WIN32)
    SET(${PROJECT_NAME}_SOURCES ${${PROJECT_NAME}_SOURCES} qtlockedfile_win.cpp)
ELSE()
    SET(${PROJECT_NAME}_SOURCES ${${PROJECT_NAME}_SOURCES} qtlockedfile_unix.cpp)
ENDIF()

# Compiler options

ADD_DEFINITIONS(-Wall -O2 -ffast-math)

# Qt specific

QT4_WRAP_CPP(${PROJECT_NAME}_SOURCES_MOC ${${PROJECT_NAME}_HEADERS_MOC})

# Rules to build QtSingleApplication itself

ADD_EXECUTABLE(${PROJECT_NAME} STATIC
    ${${PROJECT_NAME}_SOURCES}
    ${${PROJECT_NAME}_SOURCES_MOC}
)
