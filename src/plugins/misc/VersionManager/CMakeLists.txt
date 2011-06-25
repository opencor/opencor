PROJECT(VersionManagerPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/common.cmake)

# Add the plugin

ADD_PLUGIN(VersionManager FALSE
    SOURCES
        src/versionmanager.cpp
        src/versionmanagerplugin.cpp
    HEADERS_MOC
        src/versionmanager.h
        src/versionmanagerplugin.h
    PLUGIN_INCLUDE_DIRS
        src
    QT_DEPENDENCIES
        QtCore
)
