PROJECT(QtSingleApplication)

# Some in-house CMake functions/macros

INCLUDE(../../../../cmake/common.cmake)

# Initialise the plugin

INITIALISE_PLUGIN(FALSE)

# Files that make up the plugin

SET(SOURCES
    src/qtlocalpeer.cpp
    src/qtsingleapplication.cpp
)

SET(HEADERS_MOC
    src/qtlocalpeer.h
    src/qtsingleapplication.h
)

# Various include directories

SET(LOCAL_PLUGIN_INCLUDE_DIRS src)
SET(PLUGIN_INCLUDE_DIRS ${LOCAL_PLUGIN_INCLUDE_DIRS} PARENT_SCOPE)

INCLUDE_DIRECTORIES(${LOCAL_PLUGIN_INCLUDE_DIRS})

# Build the plugin

BUILD_PLUGIN(
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
)

# Package the plugin

PACKAGE_PLUGIN()
