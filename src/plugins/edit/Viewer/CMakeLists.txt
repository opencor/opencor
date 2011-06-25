PROJECT(ViewerPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/plugin.cmake)

# Initialise the plugin

INITIALISE_PLUGIN(Viewer TRUE)

# Files that make up the plugin

SET(SOURCES
    src/mmlviewerwidget.cpp
    src/viewerplugin.cpp
    src/viewerwindow.cpp
)

SET(HEADERS_MOC
    src/mmlviewerwidget.h
    src/viewerplugin.h
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
    CorePlugin
    QtMmlWidgetPlugin
)

# Set the right Qt libraries for the plugin

SET_PLUGIN_QT_LIBRARIES(
    QtCore
    QtGui
    QtXml
)

# Package the plugin

PACKAGE_PLUGIN()
