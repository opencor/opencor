PROJECT(SingleCellViewPlugin)

# Retrieve some CellML API and LLVM settings

RETRIEVE_CELLML_API_SETTINGS()
RETRIEVE_LLVM_SETTINGS()

# Add the plugin

ADD_PLUGIN(SingleCellView
    SOURCES
        ../../i18ninterface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../pluginmanager.cpp
        ../../solverinterface.cpp
        ../../viewinterface.cpp

        src/singlecellviewcontentswidget.cpp
        src/singlecellviewgraphpanelplotwidget.cpp
        src/singlecellviewgraphpanelswidget.cpp
        src/singlecellviewgraphpanelwidget.cpp
        src/singlecellviewinformationgraphswidget.cpp
        src/singlecellviewinformationparameterswidget.cpp
        src/singlecellviewinformationsimulationwidget.cpp
        src/singlecellviewinformationsolverswidget.cpp
        src/singlecellviewinformationwidget.cpp
        src/singlecellviewplugin.cpp
        src/singlecellviewsimulation.cpp
        src/singlecellviewsimulationworker.cpp
        src/singlecellviewwidget.cpp
    HEADERS_MOC
        ../../plugin.h
        ../../pluginmanager.h

        src/singlecellviewcontentswidget.h
        src/singlecellviewgraphpanelplotwidget.h
        src/singlecellviewgraphpanelswidget.h
        src/singlecellviewgraphpanelwidget.h
        src/singlecellviewinformationgraphswidget.h
        src/singlecellviewinformationparameterswidget.h
        src/singlecellviewinformationsimulationwidget.h
        src/singlecellviewinformationsolverswidget.h
        src/singlecellviewinformationwidget.h
        src/singlecellviewplugin.h
        src/singlecellviewsimulation.h
        src/singlecellviewsimulationworker.h
        src/singlecellviewwidget.h
    UIS
        src/singlecellviewgraphpanelplotwidget.ui
        src/singlecellviewgraphpanelwidget.ui
        src/singlecellviewinformationgraphswidget.ui
        src/singlecellviewinformationwidget.ui
        src/singlecellviewwidget.ui
    INCLUDE_DIRS
        src
    DEFINITIONS
        ${LLVM_DEFINITIONS}
        QWT_DLL
    PLUGINS
        CellMLAPI
        CellMLSupport
        Compiler
        Core
        CoreDataStore
        CoreSolver
        CSVDataStore
        ${LLVM_PLUGIN}
        ${QWT_PLUGIN}
    PLUGIN_BINARIES
        ${LLVM_PLUGIN_BINARY}
        ${QWT_PLUGIN_BINARY}
    QT_LIBRARIES
        QtConcurrent
        QtCore
        QtGui
        QtNetwork
        QtOpenGL
        QtPrintSupport
        QtSvg
        QtWidgets
        QtXml
        QtXmlPatterns
    EXTERNAL_BINARIES
        ${CELLML_API_EXTERNAL_BINARIES}
)
