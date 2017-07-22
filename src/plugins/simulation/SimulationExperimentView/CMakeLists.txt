PROJECT(SimulationExperimentViewPlugin)

# Add the plugin

ADD_PLUGIN(SimulationExperimentView
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

        src/simulationexperimentviewcontentswidget.cpp
        src/simulationexperimentviewinformationgraphswidget.cpp
        src/simulationexperimentviewinformationparameterswidget.cpp
        src/simulationexperimentviewinformationsimulationwidget.cpp
        src/simulationexperimentviewinformationsolverswidget.cpp
        src/simulationexperimentviewinformationwidget.cpp
        src/simulationexperimentviewplugin.cpp
        src/simulationexperimentviewsimulationwidget.cpp
        src/simulationexperimentviewwidget.cpp
    HEADERS_MOC
        ../../datastoreinterface.h
        ../../plugin.h
        ../../pluginmanager.h
        ../../solverinterface.h

        src/simulationexperimentviewcontentswidget.h
        src/simulationexperimentviewinformationgraphswidget.h
        src/simulationexperimentviewinformationparameterswidget.h
        src/simulationexperimentviewinformationsimulationwidget.h
        src/simulationexperimentviewinformationsolverswidget.h
        src/simulationexperimentviewinformationwidget.h
        src/simulationexperimentviewplugin.h
        src/simulationexperimentviewsimulationwidget.h
        src/simulationexperimentviewwidget.h
    PLUGINS
        GraphPanelWidget
        SimulationSupport
)
