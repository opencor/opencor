PROJECT(CellMLPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/common.cmake)

# Add the plugin

ADD_PLUGIN(CellML
    SOURCES
        ../../plugininterface.cpp

        src/cellmlplugin.cpp
    HEADERS
        ../../plugininterface.h
    HEADERS_MOC
        src/cellmlplugin.h
    PLUGIN_INCLUDE_DIRS
        src
    QT_DEPENDENCIES
        QtCore
        QtGui
)
