PROJECT(SingleCellSimulationPlugin)

# Retrieve some CellML settings

RETRIEVE_CELLML_SETTINGS()

# Add the plugin

ADD_PLUGIN(SingleCellSimulation
    SOURCES
        ../../coreinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/singlecellsimulationplugin.cpp
    HEADERS_MOC
        src/singlecellsimulationplugin.h
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
