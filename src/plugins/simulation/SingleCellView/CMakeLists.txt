PROJECT(SingleCellViewPlugin)

# Retrieve some CellML API and LLVM settings

RETRIEVE_CELLML_API_SETTINGS()
RETRIEVE_LLVM_SETTINGS()

# Add the plugin

ADD_PLUGIN(SingleCellView
    SOURCES
        ../../coreinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../interface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../pluginmanager.cpp
        ../../solverinterface.cpp

        src/singlecellviewcontentswidget.cpp
        src/singlecellviewgraphpanelplotwidget.cpp
        src/singlecellviewgraphpanelswidget.cpp
        src/singlecellviewgraphpanelwidget.cpp
        src/singlecellviewinformationparameterswidget.cpp
        src/singlecellviewinformationsimulationwidget.cpp
        src/singlecellviewinformationsolverswidget.cpp
        src/singlecellviewinformationgraphswidget.cpp
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
        src/singlecellviewinformationparameterswidget.h
        src/singlecellviewinformationsimulationwidget.h
        src/singlecellviewinformationsolverswidget.h
        src/singlecellviewinformationgraphswidget.h
        src/singlecellviewinformationwidget.h
        src/singlecellviewplugin.h
        src/singlecellviewsimulation.h
        src/singlecellviewsimulationworker.h
        src/singlecellviewwidget.h
    UIS
        src/singlecellviewgraphpanelwidget.ui
        src/singlecellviewinformationwidget.ui
        src/singlecellviewwidget.ui
    DEFINITIONS
        ${LLVM_DEFINITIONS}
        QWT_DLL
    INCLUDE_DIRS
        ../../3rdparty/Qwt/src

        src
    PLUGIN_DEPENDENCIES
        CellMLAPI
        CellMLSupport
        Compiler
        Core
        CoreSolver
        ${LLVM_PLUGIN}
        Qwt
    PLUGIN_BINARY_DEPENDENCIES
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
    EXTERNAL_BINARY_DEPENDENCIES
        ${CELLML_API_EXTERNAL_BINARY_DEPENDENCIES}
)
