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
        ../../pluginmanager.cpp
        ../../solverinterface.cpp

        src/singlecellsimulationviewcontentswidget.cpp
        src/singlecellsimulationviewgraphpanelwidget.cpp
        src/singlecellsimulationviewgraphpanelswidget.cpp
        src/singlecellsimulationviewinformationparameterswidget.cpp
        src/singlecellsimulationviewinformationsimulationwidget.cpp
        src/singlecellsimulationviewinformationsolverswidget.cpp
        src/singlecellsimulationviewinformationwidget.cpp
        src/singlecellsimulationviewplugin.cpp
        src/singlecellsimulationviewprogressbarwidget.cpp
        src/singlecellsimulationviewsimulation.cpp
        src/singlecellsimulationviewsimulationdata.cpp
        src/singlecellsimulationviewsimulationworker.cpp
        src/singlecellsimulationviewwidget.cpp
    HEADERS_MOC
        ../../plugin.h
        ../../pluginmanager.h

        src/singlecellsimulationviewcontentswidget.h
        src/singlecellsimulationviewgraphpanelwidget.h
        src/singlecellsimulationviewgraphpanelswidget.h
        src/singlecellsimulationviewinformationparameterswidget.h
        src/singlecellsimulationviewinformationsimulationwidget.h
        src/singlecellsimulationviewinformationsolverswidget.h
        src/singlecellsimulationviewinformationwidget.h
        src/singlecellsimulationviewplugin.h
        src/singlecellsimulationviewprogressbarwidget.h
        src/singlecellsimulationviewsimulation.h
        src/singlecellsimulationviewsimulationworker.h
        src/singlecellsimulationviewwidget.h
    UIS
        src/singlecellsimulationviewcontentswidget.ui
        src/singlecellsimulationviewgraphpanelwidget.ui
        src/singlecellsimulationviewinformationwidget.ui
        src/singlecellsimulationviewwidget.ui
    DEFINITIONS
        ${LLVM_DEFINITIONS}
    INCLUDE_DIRS
        ../../3rdparty/Qwt/src

        src
    OPENCOR_DEPENDENCIES
        CellMLAPI
        CellMLSupport
        Compiler
        Core
        CoreSolver
        ${LLVM_PLUGIN}
        Qwt
    OPENCOR_BINARY_DEPENDENCIES
        ${LLVM_BINARY_PLUGIN}
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtNetwork
        QtSvg
        QtXml
    EXTERNAL_DEPENDENCIES
        ${CELLML_API_EXTERNAL_DEPENDENCIES}
)
