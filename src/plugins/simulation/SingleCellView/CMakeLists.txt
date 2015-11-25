PROJECT(SingleCellViewPlugin)

# Retrieve the CellML, SBML and SED-ML APIs' external binaries

RETRIEVE_CELLML_API_EXTERNAL_BINARIES()
RETRIEVE_SBML_API_EXTERNAL_BINARIES()
RETRIEVE_SEDML_API_EXTERNAL_BINARIES()

# Apply LLVM's, Qwt's and zlib's build settings

APPLY_LLVM_BUILD_SETTINGS()
APPLY_QWT_BUILD_SETTINGS()
APPLY_ZLIB_BUILD_SETTINGS()

# Add the plugin

ADD_PLUGIN(SingleCellView
    SOURCES
        ../../datastoreinterface.cpp
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
        ../../solverinterface.h

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
        src/singlecellviewinformationgraphswidget.ui
        src/singlecellviewwidget.ui
    INCLUDE_DIRS
        src
    DEFINITIONS
        ${LLVM_DEFINITIONS}
        ${QWT_DEFINITIONS}
        ${ZLIB_DEFINITIONS}
    PLUGINS
        CellMLAPI
        CellMLSupport
        COMBINESupport
        Compiler
        Core
        ${LLVM_PLUGIN}
        ${QWT_PLUGIN}
        SBMLAPI
        SEDMLAPI
        SEDMLSupport
        ${ZLIB_PLUGIN}
    PLUGIN_BINARIES
        ${LLVM_PLUGIN_BINARY}
        ${QWT_PLUGIN_BINARY}
        ${ZLIB_PLUGIN_BINARY}
    EXTERNAL_BINARIES
        ${CELLML_API_EXTERNAL_BINARIES}
        ${SBML_API_EXTERNAL_BINARIES}
        ${SEDML_API_EXTERNAL_BINARIES}
)
