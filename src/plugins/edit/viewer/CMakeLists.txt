PROJECT(Viewer)

# Some in-house CMake functions/macros

INCLUDE(../../../../cmake/common.cmake)

# Initialise the plugin

INITIALISE_PLUGIN(TRUE)

# Files that make up the plugin

SET(SOURCES
    src/mmlviewerwidget.cpp
    src/viewerwindow.cpp
)

SET(HEADERS_MOC
    src/mmlviewerwidget.h
    src/viewerwindow.h
)

SET(UIS
    src/viewerwindow.ui
)

# Various include directories

SET(LOCAL_PLUGIN_INCLUDE_DIRS src)
SET(PLUGIN_INCLUDE_DIRS ${LOCAL_PLUGIN_INCLUDE_DIRS} PARENT_SCOPE)

INCLUDE_DIRECTORIES(${LOCAL_PLUGIN_INCLUDE_DIRS})

# Build the plugin

BUILD_PLUGIN(
    QtMmlWidget

    Core
)

# Package the plugin

PACKAGE_PLUGIN()
