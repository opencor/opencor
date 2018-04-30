project(SimulationSupportPlugin)

# Add the plugin

add_plugin(SimulationSupport
    SOURCES
        ../../datastoreinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../simulationinterface.cpp
        ../../solverinterface.cpp

        src/simulation.cpp
        src/simulationmanager.cpp
        src/simulationsupportplugin.cpp
        src/simulationworker.cpp
    HEADERS_MOC
        ../../datastoreinterface.h
        ../../solverinterface.h

        src/simulation.h
        src/simulationmanager.h
        src/simulationsupportplugin.h
        src/simulationworker.h
    PLUGINS
        COMBINESupport
)
