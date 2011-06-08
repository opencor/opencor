PROJECT(Viewer)

# Some in-house CMake functions/macros

INCLUDE(../../../../cmake/common.cmake)

# Initialise the plugin

INITIALISE_PLUGIN(TRUE)

# Files that make up the Viewer plugin

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

BUILD_PLUGIN(${PROJECT_NAME}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}

    Core
    QtMmlWidget
)

# Package the plugin

PACKAGE_PLUGIN()
