PROJECT(CoreSimulationPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/common.cmake)

# Add the plugin

ADD_PLUGIN(CoreSimulation
    SOURCES
        ../../plugininterface.cpp

        src/coresimulationplugin.cpp
    HEADERS
        ../../plugininterface.h
    HEADERS_MOC
        src/coresimulationplugin.h
    PLUGIN_INCLUDE_DIRS
        src
    QT_DEPENDENCIES
        QtCore
        QtGui
)
