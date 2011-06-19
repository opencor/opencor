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

# Third-party library which must be directly embedded in the console version of
# OpenCOR

SET(MAIN_SOURCE_LOCATION ..)

SET(THIRD_PARTY_LIBRARIES
    LibQxt
)

FOREACH(THIRD_PARTY_LIBRARY ${THIRD_PARTY_LIBRARIES})
    INCLUDE(${MAIN_SOURCE_LOCATION}/src/3rdparty/${THIRD_PARTY_LIBRARY}/${THIRD_PARTY_LIBRARY}.cmake)
ENDFOREACH()

# Qt specific

SET(QT_DONT_USE_QTGUI TRUE)
# Note: since we want a console version of OpenCOR...

INCLUDE(${QT_USE_FILE})

# Build the console version of OpenCOR

QT4_ADD_RESOURCES(SOURCES_RCS ${RESOURCES})

ADD_EXECUTABLE(${PROJECT_NAME} WIN32
    ${SOURCES}
    ${SOURCES_RCS}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    ${QT_QTCORE_LIBRARY}
)

# Additional compiler and linker settings

SET(CMAKE_CXX_FLAGS "-Wall ${CMAKE_CXX_FLAGS}")

SET(LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES} -Wl,-subsystem,console")
# Note: -Wl,-subsystem,console makes sure that we end up with a console
#       application

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${LINK_FLAGS_PROPERTIES}")

# Copy the console version of OpenCOR to the main build folder

ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.exe ../../build/${PROJECT_NAME}.com)
