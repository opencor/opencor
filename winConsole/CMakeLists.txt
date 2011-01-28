PROJECT(OpenCOR)

# Minimum version of CMake required

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# Files that make up the console version of OpenCOR

SET(SOURCES
    src/main.cpp
    ../src/misc/cmdLine.cpp
)

SET(HEADERS
    ../src/misc/cmdLine.h
)

# Various include directories

INCLUDE_DIRECTORIES(../src/misc)

# Qt specific

FIND_PACKAGE(Qt4 4.6.0 REQUIRED)

SET(QT_USE_QTMAIN TRUE)
# Note: the above ensures that winmain@16 is available on Windows

SET(QT_DONT_USE_QTGUI TRUE)
# Note: since we want a console version of OpenCOR...

INCLUDE(${QT_USE_FILE})

# Build the third-party libraries used by the console version of OpenCOR

SET(3RDPARTYLIBS LibQxt)

FOREACH(3RDPARTYLIB ${3RDPARTYLIBS})
    SET(3RDPARTYLIB_DIR ../3rdparty/${3RDPARTYLIB})
    SET(3RDPARTYLIB_SUBDIRS)

    ADD_SUBDIRECTORY(${3RDPARTYLIB_DIR} 3rdparty/${3RDPARTYLIB})

    IF("${3RDPARTYLIB_SUBDIRS}" STRGREATER "")
        # The third-party library has at least one sub-directory, so add
        # it/them

        FOREACH(3RDPARTYLIB_SUBDIR ${3RDPARTYLIB_SUBDIRS})
            INCLUDE_DIRECTORIES(${3RDPARTYLIB_DIR}/${3RDPARTYLIB_SUBDIR})
        ENDFOREACH()
    ELSE()
        # The third-party library doesn't have any sub-directory, so just add
        # the third-party library directory and that's it

        INCLUDE_DIRECTORIES(${3RDPARTYLIB_DIR})
    ENDIF()
ENDFOREACH()

# Rules to build the console version of OpenCOR

#SET(CMAKE_VERBOSE_MAKEFILE ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

ADD_EXECUTABLE(${PROJECT_NAME} WIN32
    ${SOURCES}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    ${QT_QTCORE_LIBRARY}

    ${3RDPARTYLIBS}
)

# Compiler and linker options

ADD_DEFINITIONS(-Wall -O2 -ffast-math -enable-auto-import)

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-subsystem,console,--enable-auto-import")
# Note: -subsystem,console ensures that we end up with a console application
