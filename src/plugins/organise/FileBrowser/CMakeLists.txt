PROJECT(FileBrowserPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/plugin.cmake)

# Initialise the plugin

INITIALISE_PLUGIN(FileBrowser TRUE)

# Files that make up the plugin

SET(SOURCES
    src/filebrowserplugin.cpp
    src/filebrowserwidget.cpp
    src/filebrowserwindow.cpp
)

SET(HEADERS_MOC
    src/filebrowserplugin.h
    src/filebrowserwidget.h
    src/filebrowserwindow.h
)

SET(UIS
    src/filebrowserwindow.ui
)

# Various include directories

SET(LOCAL_PLUGIN_INCLUDE_DIRS src)
SET(PLUGIN_INCLUDE_DIRS ${LOCAL_PLUGIN_INCLUDE_DIRS} PARENT_SCOPE)

INCLUDE_DIRECTORIES(${LOCAL_PLUGIN_INCLUDE_DIRS})

# Build the plugin

BUILD_PLUGIN(
    CorePlugin
)

# Sort out our plugin and Qt dependencies

SET_PLUGIN_OPENCOR_DEPENDENCIES(
    Core
)

SET_PLUGIN_QT_DEPENDENCIES(
    QtCore
    QtGui
)

# Package the plugin

PACKAGE_PLUGIN()
