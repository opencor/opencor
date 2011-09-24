PROJECT(CoreCellMLEditingPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/common.cmake)

# Add the plugin

ADD_PLUGIN(CoreCellMLEditing
    SOURCES
        ../../coreinterface.cpp

        src/corecellmleditingplugin.cpp
    HEADERS
        ../../coreinterface.h
    HEADERS_MOC
        src/corecellmleditingplugin.h
    PLUGIN_INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        Core
    QT_DEPENDENCIES
        QtCore
        QtGui
)
