PROJECT(CellMLSimulationPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/common.cmake)

# Add the plugin

ADD_PLUGIN(CellMLSimulation
    SOURCES
        ../../guiinterface.cpp
        ../../coreinterface.cpp
        ../../plugininfo.cpp

        src/cellmlsimulationplugin.cpp
    HEADERS
        ../../guiinterface.h
        ../../coreinterface.h
        ../../plugininfo.h
    HEADERS_MOC
        src/cellmlsimulationplugin.h
    PLUGIN_INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        Core
    QT_DEPENDENCIES
        QtCore
        QtGui
)
