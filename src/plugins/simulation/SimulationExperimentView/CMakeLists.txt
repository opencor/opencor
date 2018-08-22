project(SimulationExperimentViewPlugin)

# Add the plugin

add_plugin(SimulationExperimentView
    SOURCES
        ../../datastoreinterface.cpp
        ../../filehandlinginterface.cpp
        ../../i18ninterface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../preferencesinterface.cpp
        ../../pluginmanager.cpp
        ../../solverinterface.cpp
        ../../viewinterface.cpp

        src/simulationexperimentviewcontentswidget.cpp
        src/simulationexperimentviewinformationgraphpanelandgraphswidget.cpp
        src/simulationexperimentviewinformationparameterswidget.cpp
        src/simulationexperimentviewinformationsimulationwidget.cpp
        src/simulationexperimentviewinformationsolverswidget.cpp
        src/simulationexperimentviewinformationwidget.cpp
        src/simulationexperimentviewplugin.cpp
        src/simulationexperimentviewpreferenceswidget.cpp
        src/simulationexperimentviewsimulationwidget.cpp
        src/simulationexperimentviewwidget.cpp
    HEADERS_MOC
        ../../datastoreinterface.h
        ../../plugin.h
        ../../pluginmanager.h
        ../../preferencesinterface.h
        ../../solverinterface.h

        src/simulationexperimentviewcontentswidget.h
        src/simulationexperimentviewinformationgraphpanelandgraphswidget.h
        src/simulationexperimentviewinformationparameterswidget.h
        src/simulationexperimentviewinformationsimulationwidget.h
        src/simulationexperimentviewinformationsolverswidget.h
        src/simulationexperimentviewinformationwidget.h
        src/simulationexperimentviewplugin.h
        src/simulationexperimentviewpreferenceswidget.h
        src/simulationexperimentviewsimulationwidget.h
        src/simulationexperimentviewwidget.h
    UIS
        src/simulationexperimentviewpreferenceswidget.ui
    PLUGINS
        GraphPanelWidget
        SimulationSupport
)
