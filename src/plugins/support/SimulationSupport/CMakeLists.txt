project(SimulationSupportPlugin)

# Add the plugin

add_plugin(SimulationSupport
    SOURCES
        ../../datastoreinterface.cpp
        ../../filehandlinginterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../pythoninterface.cpp
        ../../solverinterface.cpp

        src/simulation.cpp
        src/simulationmanager.cpp
        src/simulationsupportplugin.cpp
        src/simulationsupportpythonwrapper.cpp
        src/simulationworker.cpp
    PLUGINS
        COMBINESupport
        DataStore
        PythonQtSupport
)
