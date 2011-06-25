PROJECT(HelpPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/plugin.cmake)

# Initialise the plugin

INITIALISE_PLUGIN(Help TRUE)

# Files that make up the plugin

SET(SOURCES
    src/helpplugin.cpp
    src/helpwidget.cpp
    src/helpwindow.cpp
)

SET(HEADERS_MOC
    src/helpplugin.h
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

BUILD_PLUGIN(
    CorePlugin

    ${QT_QTHELP_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
    ${QT_QTWEBKIT_LIBRARY}
)

# Sort out our plugin and Qt dependencies

SET_PLUGIN_OPENCOR_DEPENDENCIES(
    Core
)

SET_PLUGIN_QT_DEPENDENCIES(
    QtCore
    QtGui
    QtHelp
    QtNetwork
    QtWebKit
)

# Package the plugin

PACKAGE_PLUGIN()
