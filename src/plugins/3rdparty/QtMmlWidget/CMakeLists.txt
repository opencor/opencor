PROJECT(QtMmlWidgetPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/plugin.cmake)

# Initialise the plugin

INITIALISE_PLUGIN(QtMmlWidget FALSE)

# Files that make up the plugin

SET(SOURCES
    src/qtmmlwidget.cpp
)

SET(HEADERS
    src/qtmmlwidget.h
)

# Various include directories

SET(LOCAL_PLUGIN_INCLUDE_DIRS include src)
SET(PLUGIN_INCLUDE_DIRS ${LOCAL_PLUGIN_INCLUDE_DIRS} PARENT_SCOPE)

INCLUDE_DIRECTORIES(${LOCAL_PLUGIN_INCLUDE_DIRS})

# Build the plugin

BUILD_PLUGIN(
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTXML_LIBRARY}
)

# Package the plugin

PACKAGE_PLUGIN()
