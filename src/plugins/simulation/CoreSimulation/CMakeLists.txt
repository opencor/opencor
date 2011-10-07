PROJECT(CoreSimulationPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/common.cmake)

# Add the plugin

ADD_PLUGIN(CoreSimulation
    SOURCES
        ../../coreinterface.cpp
        ../../guiinterface.cpp
        ../../plugininfo.cpp

        src/coresimulationplugin.cpp
    HEADERS
        ../../coreinterface.h
        ../../guiinterface.h
        ../../plugininfo.h
    HEADERS_MOC
        src/coresimulationplugin.h
    PLUGIN_INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        Core
        Qwt
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtSvg
        QtXml
)
