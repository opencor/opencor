PROJECT(Core)

# Some in-house CMake functions/macros

INCLUDE(../../../cmake/common.cmake)

# Initialise the plugin

INITIALISE_PLUGIN(FALSE)

# Files that make up the Core plugin

SET(SOURCES
    src/commonwidget.cpp
    src/dockwidget.cpp
)

SET(HEADERS
    src/commonwidget.h
)

SET(HEADERS_MOC
    src/dockwidget.h
)

# Various include directories

SET(LOCAL_PLUGIN_INCLUDE_DIRS src)
SET(PLUGIN_INCLUDE_DIRS ${LOCAL_PLUGIN_INCLUDE_DIRS} PARENT_SCOPE)

INCLUDE_DIRECTORIES(${LOCAL_PLUGIN_INCLUDE_DIRS})

# Build the plugin

BUILD_PLUGIN(${PROJECT_NAME} FALSE
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
)

# Package the plugin

PACKAGE_PLUGIN()
