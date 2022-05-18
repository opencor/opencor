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
        ../../pythoninterface.cpp
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
        src/simulationexperimentviewpythonwrapper.cpp
        src/simulationexperimentviewsedmlsupport.cpp
        src/simulationexperimentviewsimulationwidget.cpp
        src/simulationexperimentviewwidget.cpp
    UIS
        src/simulationexperimentviewpreferenceswidget.ui
    PLUGINS
        GraphPanelWidget
        PythonQtSupport
        SimulationSupport
        ToolBarWidget
    TESTS
        tests
)
