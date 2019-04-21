project(SimulationSupportPlugin)

# Add the plugin

add_plugin(SimulationSupport
    SOURCES
        ../../datastoreinterface.cpp
        ../../filehandlinginterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../solverinterface.cpp

        src/simulation.cpp
        src/simulationmanager.cpp
        src/simulationsupportplugin.cpp
        src/simulationworker.cpp
    PLUGINS
        COMBINESupport
)
