PROJECT(CellMLPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/plugin.cmake)

# Initialise the plugin

INITIALISE_PLUGIN(CellML FALSE)

# Files that make up the plugin

SET(SOURCES
    src/cellml.cpp
    src/cellmlplugin.cpp
)

SET(HEADERS
    src/cellml.h
)

SET(HEADERS_MOC
    src/cellmlplugin.h
)

# Various include directories

SET(LOCAL_PLUGIN_INCLUDE_DIRS src)
SET(PLUGIN_INCLUDE_DIRS ${LOCAL_PLUGIN_INCLUDE_DIRS} PARENT_SCOPE)

INCLUDE_DIRECTORIES(${LOCAL_PLUGIN_INCLUDE_DIRS})

# Build the plugin

BUILD_PLUGIN(
    ${QT_QTCORE_LIBRARY}
)

# Package the plugin

PACKAGE_PLUGIN()
