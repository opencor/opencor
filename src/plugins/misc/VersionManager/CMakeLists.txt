PROJECT(VersionManagerPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/plugin.cmake)

# Initialise the plugin

INITIALISE_PLUGIN(VersionManager FALSE)

# Files that make up the plugin

SET(SOURCES
    src/versionmanager.cpp
    src/versionmanagerplugin.cpp
)

SET(HEADERS_MOC
    src/versionmanager.h
    src/versionmanagerplugin.h
)

# Various include directories

SET(LOCAL_PLUGIN_INCLUDE_DIRS src)
SET(PLUGIN_INCLUDE_DIRS ${LOCAL_PLUGIN_INCLUDE_DIRS} PARENT_SCOPE)

INCLUDE_DIRECTORIES(${LOCAL_PLUGIN_INCLUDE_DIRS})

# Build the plugin

BUILD_PLUGIN(
    ${QT_QTCORE_LIBRARY}
)

# Sort out our plugin and Qt dependencies

SET_PLUGIN_OPENCOR_DEPENDENCIES(
)

SET_PLUGIN_QT_DEPENDENCIES(
    QtCore
)

# Package the plugin

PACKAGE_PLUGIN()
