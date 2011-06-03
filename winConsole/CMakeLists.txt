PROJECT(OpenCOR)

# Minimum version of CMake required

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# Files that make up the console version of OpenCOR

SET(SOURCES
    src/main.cpp

    ../src/misc/common.cpp
    ../src/misc/utils.cpp
)

SET(HEADERS
    ../src/misc/common.h
    ../src/misc/utils.h
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

# Type of build
# Note: we do this here, so that all of our third-party libraries can benefit
#       from these settings

IF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    SET(DEBUG_MODE ON)
ELSE()
    SET(DEBUG_MODE OFF)
ENDIF()

# Build the third-party libraries used by the console version of OpenCOR

SET(THIRD_PARTY_LIBRARIES LibQxt)

FOREACH(THIRD_PARTY_LIBRARY ${THIRD_PARTY_LIBRARIES})
    SET(THIRD_PARTY_LIBRARY_DIR ../3rdparty/${THIRD_PARTY_LIBRARY})
    SET(THIRD_PARTY_LIBRARY_INCLUDE_DIRS)
    # Note: the above variable is defined here, but is actually set when
    #       processing the CMakeLists.txt file of the third-party library

    ADD_SUBDIRECTORY(${THIRD_PARTY_LIBRARY_DIR} 3rdparty/${THIRD_PARTY_LIBRARY})

    FOREACH(THIRD_PARTY_LIBRARY_INCLUDE_DIR ${THIRD_PARTY_LIBRARY_INCLUDE_DIRS})
        INCLUDE_DIRECTORIES(${THIRD_PARTY_LIBRARY_DIR}/${THIRD_PARTY_LIBRARY_INCLUDE_DIR})
    ENDFOREACH()
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

    ${THIRD_PARTY_LIBRARIES}
)

# Compiler and linker options

SET(CMAKE_CXX_FLAGS "-Wall")
SET(LINK_FLAGS_PROPERTIES)

IF(${DEBUG_MODE})
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
ELSE()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -ffast-math")
ENDIF()

SET(LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES} -Wl,-enable-auto-import")
# Note: -Wl,-enable-auto-import allows us to use our various third-party
#       libraries without having to worry about importing their functions, etc.

IF(NOT ${DEBUG_MODE})
    SET(LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES} -Wl,-s")
    # Note: -Wl,-s strips all the symbols, thus reducing the final size of
    #       our console application
ENDIF()

SET(LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES} -Wl,-subsystem,console")
# Note: -Wl,-subsystem,console makes sure that we end up with a console
#       application

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${LINK_FLAGS_PROPERTIES}")

# Move the console version of OpenCOR to the main build folder

ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_NAME}.exe ../../build/${PROJECT_NAME}.com
                   WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
