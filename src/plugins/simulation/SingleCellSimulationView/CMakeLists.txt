PROJECT(SingleCellSimulationViewPlugin)

# Retrieve some CellML API and LLVM settings

RETRIEVE_CELLML_API_SETTINGS()
RETRIEVE_LLVM_SETTINGS()

# Add the plugin

ADD_PLUGIN(SingleCellSimulationView
    SOURCES
        ../../coreinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../interface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../solverinterface.cpp

        src/singlecellsimulationviewgraphpanel.cpp
        src/singlecellsimulationviewgraphpanels.cpp
        src/singlecellsimulationviewplugin.cpp
        src/singlecellsimulationviewwidget.cpp
    HEADERS_MOC
        ../../plugin.h

        src/singlecellsimulationviewgraphpanel.h
        src/singlecellsimulationviewgraphpanels.h
        src/singlecellsimulationviewplugin.h
        src/singlecellsimulationviewwidget.h
    UIS
        src/singlecellsimulationviewwidget.ui
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
        CoreSolver
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
