PROJECT(SingleCellSimulationViewPlugin)

# Retrieve some CellML API, LLVM and Qwt settings

RETRIEVE_CELLML_API_SETTINGS()
RETRIEVE_LLVM_SETTINGS()
RETRIEVE_QWT_SETTINGS()

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
        src/singlecellsimulationviewinformationtraceswidget.cpp
        src/singlecellsimulationviewinformationwidget.cpp
        src/singlecellsimulationviewplugin.cpp
        src/singlecellsimulationviewsimulation.cpp
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
        src/singlecellsimulationviewinformationtraceswidget.h
        src/singlecellsimulationviewinformationwidget.h
        src/singlecellsimulationviewplugin.h
        src/singlecellsimulationviewsimulation.h
        src/singlecellsimulationviewsimulationworker.h
        src/singlecellsimulationviewwidget.h
    UIS
        src/singlecellsimulationviewgraphpanelwidget.ui
        src/singlecellsimulationviewinformationwidget.ui
        src/singlecellsimulationviewwidget.ui
    DEFINITIONS
        ${LLVM_DEFINITIONS}
        QWT_DLL
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
    QT_MODULES
        Widgets
    QT_DEPENDENCIES
        QtConcurrent
        QtCore
        QtGui
        QtNetwork
        QtOpenGL
        QtPrintSupport
        QtSvg
        QtWidgets
        QtXml
    EXTERNAL_DEPENDENCIES
        ${CELLML_API_EXTERNAL_DEPENDENCIES}
)
