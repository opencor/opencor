PROJECT(CoreSimulationPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/common.cmake)

# Add the plugin

ADD_PLUGIN(CoreSimulation
    SOURCES
        ../../plugininfo.cpp

        src/coresimulationplugin.cpp
    HEADERS
        ../../plugininfo.h
    HEADERS_MOC
        src/coresimulationplugin.h
    PLUGIN_INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        Core
    QT_DEPENDENCIES
        QtCore
        QtGui
)
