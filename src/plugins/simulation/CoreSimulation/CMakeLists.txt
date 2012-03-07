PROJECT(CoreSimulationPlugin)

# Add the plugin

ADD_PLUGIN(CoreSimulation
    SOURCES
        ../../plugininfo.cpp

        src/coresimulationplugin.cpp
    HEADERS_MOC
        src/coresimulationplugin.h
    INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        CoreSolver
    QT_DEPENDENCIES
        QtCore
        QtGui
)
