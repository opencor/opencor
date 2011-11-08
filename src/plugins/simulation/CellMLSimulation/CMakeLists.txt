PROJECT(CellMLSimulationPlugin)

# Add the plugin

ADD_PLUGIN(CellMLSimulation
    SOURCES
        ../../plugininfo.cpp

        src/cellmlsimulationplugin.cpp
    HEADERS
        ../../plugininfo.h
    HEADERS_MOC
        src/cellmlsimulationplugin.h
    INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        Core
    QT_DEPENDENCIES
        QtCore
        QtGui
)
