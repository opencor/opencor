PROJECT(SingleCellViewPlugin)

# Add the plugin

ADD_PLUGIN(SingleCellView
    SOURCES
        ../../datastoreinterface.cpp
        ../../filehandlinginterface.cpp
        ../../i18ninterface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../pluginmanager.cpp
        ../../solverinterface.cpp
        ../../viewinterface.cpp

        src/singlecellviewcontentswidget.cpp
        src/singlecellviewinformationgraphswidget.cpp
        src/singlecellviewinformationparameterswidget.cpp
        src/singlecellviewinformationsimulationwidget.cpp
        src/singlecellviewinformationsolverswidget.cpp
        src/singlecellviewinformationwidget.cpp
        src/singlecellviewplugin.cpp
        src/singlecellviewsimulation.cpp
        src/singlecellviewsimulationworker.cpp
        src/singlecellviewsimulationwidget.cpp
        src/singlecellviewwidget.cpp
    HEADERS_MOC
        ../../datastoreinterface.h
        ../../plugin.h
        ../../pluginmanager.h
        ../../solverinterface.h

        src/singlecellviewcontentswidget.h
        src/singlecellviewinformationgraphswidget.h
        src/singlecellviewinformationparameterswidget.h
        src/singlecellviewinformationsimulationwidget.h
        src/singlecellviewinformationsolverswidget.h
        src/singlecellviewinformationwidget.h
        src/singlecellviewplugin.h
        src/singlecellviewsimulation.h
        src/singlecellviewsimulationworker.h
        src/singlecellviewsimulationwidget.h
        src/singlecellviewwidget.h
    INCLUDE_DIRS
        src
    PLUGINS
        COMBINESupport
        GraphPanelWidget
        SEDMLSupport
)
