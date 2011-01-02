PROJECT(QtSingleApplication)

# Minimum version of CMake required

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# Type of build

SET(CMAKE_BUILD_TYPE Release)

# Files that make up QtSingleApplication

SET(${PROJECT_NAME}_SOURCES
    qtlocalpeer.cpp
    qtsingleapplication.cpp
)

SET(${PROJECT_NAME}_HEADERS_MOC
    qtlocalpeer.h
    qtsingleapplication.h
)

# Compiler options

ADD_DEFINITIONS(-w -O2 -ffast-math -DQT_QTSINGLEAPPLICATION_EXPORT)

# Qt specific

FIND_PACKAGE(Qt4 4.7 REQUIRED)

INCLUDE(${QT_USE_FILE})

QT4_WRAP_CPP(${PROJECT_NAME}_SOURCES_MOC ${${PROJECT_NAME}_HEADERS_MOC})

# Rules to build QtSingleApplication

IF(APPLE)
    SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
ELSE()
    SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
ENDIF()
# Note: the above is only required so that we can quickly test OpenCOR without
#       first having to package everything

ADD_LIBRARY(${PROJECT_NAME} SHARED
    ${${PROJECT_NAME}_SOURCES}
    ${${PROJECT_NAME}_SOURCES_MOC}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
)

# Rules to package QtSingleApplication

IF(WIN32)
    INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
ELSEIF(NOT APPLE)
    INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
ENDIF()
