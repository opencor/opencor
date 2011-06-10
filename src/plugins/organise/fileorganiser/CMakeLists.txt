PROJECT(FileOrganiser)

# Some in-house CMake functions/macros

INCLUDE(../../../../cmake/common.cmake)

# Initialise the plugin

INITIALISE_PLUGIN(TRUE)

# Files that make up the plugin

SET(SOURCES
    src/fileorganiserwidget.cpp
    src/fileorganiserwindow.cpp
)

SET(HEADERS_MOC
    src/fileorganiserwidget.h
    src/fileorganiserwindow.h
)

SET(UIS
    src/fileorganiserwindow.ui
)

# Various include directories

SET(LOCAL_PLUGIN_INCLUDE_DIRS src)
SET(PLUGIN_INCLUDE_DIRS ${LOCAL_PLUGIN_INCLUDE_DIRS} PARENT_SCOPE)

INCLUDE_DIRECTORIES(${LOCAL_PLUGIN_INCLUDE_DIRS})

# Build the plugin

BUILD_PLUGIN(${PROJECT_NAME}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTHELP_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
    ${QT_QTSQL_LIBRARY}
    ${QT_QTWEBKIT_LIBRARY}
    ${QT_QTXML_LIBRARY}

    Core
)

# Package the plugin

PACKAGE_PLUGIN()
