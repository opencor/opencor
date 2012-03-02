PROJECT(SingleCellSimulationPlugin)

# Retrieve some CellML API and LLVM settings

RETRIEVE_CELLML_API_SETTINGS()
RETRIEVE_LLVM_SETTINGS()

# Add the plugin

ADD_PLUGIN(SingleCellSimulation
    SOURCES
        ../../coreinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/singlecellsimulationgraphpanel.cpp
        src/singlecellsimulationplugin.cpp
        src/singlecellsimulationview.cpp
    HEADERS_MOC
        src/singlecellsimulationgraphpanel.h
        src/singlecellsimulationplugin.h
        src/singlecellsimulationview.h
    UIS
        src/singlecellsimulationview.ui
    DEFINITIONS
        ${LLVM_DEFINITIONS}
    INCLUDE_DIRS
        ../../3rdparty/Qwt/src

        src
    OPENCOR_DEPENDENCIES
        CellMLAPI
        CellMLSupport
        Computer
        Core
        LLVM
        Qwt
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtSvg
        QtXml
    EXTERNAL_DEPENDENCIES
        ${CELLML_API_EXTERNAL_DEPENDENCIES}
        ${LLVM_EXTERNAL_DEPENDENCIES}
)
