PROJECT(OpenCOR)

# Minimum version of CMake required

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# Location of our CMake files

SET(CMAKE_FILES_DIR ${CMAKE_SOURCE_DIR}/../cmake)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/common.cmake)

# Initialise the project

INITIALISE_PROJECT()

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
    ../res/common.qrc
)

# Various include directories

INCLUDE_DIRECTORIES(../src/misc)

# Qt specific

SET(QT_DONT_USE_QTGUI TRUE)
# Note: since we want a console version of OpenCOR...

INCLUDE(${QT_USE_FILE})

# Build the plugins used by the console version of OpenCOR

SET(PLUGINS
    3rdparty/LibQxt
)

FOREACH(PLUGIN ${PLUGINS})
    SET(PLUGIN_DIR ../src/plugins/${PLUGIN})
    SET(PLUGIN_INCLUDE_DIRS)
    # Note: the above variable is defined here, but is actually set when
    #       processing the CMakeLists.txt file of the plugin

    ADD_SUBDIRECTORY(${PLUGIN_DIR} src/${PLUGIN})

    FOREACH(PLUGIN_INCLUDE_DIR ${PLUGIN_INCLUDE_DIRS})
        INCLUDE_DIRECTORIES(${PLUGIN_DIR}/${PLUGIN_INCLUDE_DIR})
    ENDFOREACH()
ENDFOREACH()

# Build the console version of OpenCOR

QT4_ADD_RESOURCES(SOURCES_RCS ${RESOURCES})

ADD_EXECUTABLE(${PROJECT_NAME} WIN32
    ${SOURCES}
    ${SOURCES_RCS}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    ${QT_QTCORE_LIBRARY}

    #---GRY---
    # The following libraries are plugins, so they shouldn't be listed here,
    # but for now they have to be since OpenCOR doesn't yet support plugins as
    # such

    LibQxtPlugin
)

# Additional compiler and linker settings

SET(CMAKE_CXX_FLAGS "-Wall ${CMAKE_CXX_FLAGS}")

SET(LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES} -Wl,-subsystem,console")
# Note: -Wl,-subsystem,console makes sure that we end up with a console
#       application

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${LINK_FLAGS_PROPERTIES}")

# Move the console version of OpenCOR to the main build folder

ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_NAME}.exe ../../build/${PROJECT_NAME}.com
                   WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
