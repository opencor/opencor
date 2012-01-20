PROJECT(CellMLSimulationPlugin)

# Retrieve some CellML settings

RETRIEVE_CELLML_SETTINGS()

# Add the plugin

ADD_PLUGIN(CellMLSimulation
    SOURCES
        ../../coreinterface.cpp
        ../../guiinterface.cpp
        ../../plugininfo.cpp

        src/cellmlsimulationplugin.cpp
    HEADERS_MOC
        src/cellmlsimulationplugin.h
    INCLUDE_DIRS
        ../../3rdparty/Qwt/src

        src
    OPENCOR_DEPENDENCIES
        CellMLSupport
        Core
        Qwt
    QT_DEPENDENCIES
        QtCore
        QtGui
    EXTERNAL_DEPENDENCIES
        ${CELLML_EXTERNAL_DEPENDENCIES}
)
