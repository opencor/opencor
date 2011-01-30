PROJECT(OpenCOR)

# Minimum version of CMake required

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# Files that make up the console version of OpenCOR

SET(SOURCES
    src/main.cpp
    ../src/misc/common.cpp
)

SET(HEADERS
    ../src/misc/common.h
)

SET(RESOURCES
    res/${PROJECT_NAME}.qrc
)

# Various include directories

INCLUDE_DIRECTORIES(../src/misc)

# Qt specific

FIND_PACKAGE(Qt4 4.6.0 REQUIRED)

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

QT4_ADD_RESOURCES(SOURCES_RCS ${RESOURCES})

ADD_EXECUTABLE(${PROJECT_NAME} WIN32
    ${SOURCES}
    ${SOURCES_RCS}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    ${QT_QTCORE_LIBRARY}

    ${3RDPARTYLIBS}
)

# Compiler and linker options

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-subsystem,console -Wl,-s")
# Note #1: -Wl,-subsystem,console makes sure that we end up with a console
#          application
# Note #2: -Wl,-s strips all the symbols, thus reducing the final size of our
#          console application

# Move the console version of OpenCOR to the main build folder

ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_NAME}.exe ../../build/${PROJECT_NAME}.com
                   WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
