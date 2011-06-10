PROJECT(Help)

# Some in-house CMake functions/macros

INCLUDE(../../../cmake/common.cmake)

# Initialise the plugin

INITIALISE_PLUGIN(TRUE)

# Files that make up the plugin

SET(SOURCES
    src/helpwidget.cpp
    src/helpwindow.cpp
)

SET(HEADERS_MOC
    src/helpwidget.h
    src/helpwindow.h
)

SET(UIS
    src/helpwindow.ui
)

# Various include directories

SET(LOCAL_PLUGIN_INCLUDE_DIRS src)
SET(PLUGIN_INCLUDE_DIRS ${LOCAL_PLUGIN_INCLUDE_DIRS} PARENT_SCOPE)

INCLUDE_DIRECTORIES(${LOCAL_PLUGIN_INCLUDE_DIRS})

# Build the plugin

BUILD_PLUGIN(${PROJECT_NAME}
    ${QT_QTHELP_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
    ${QT_QTWEBKIT_LIBRARY}

    Core
)

# Package the plugin

PACKAGE_PLUGIN()
