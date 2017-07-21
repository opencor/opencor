PROJECT(SimulationSupportPlugin)

# Add the plugin

ADD_PLUGIN(SimulationSupport
    SOURCES
        ../../datastoreinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../solverinterface.cpp

        src/simulation.cpp
        src/simulationsupportplugin.cpp
        src/simulationworker.cpp
    HEADERS_MOC
        ../../datastoreinterface.h
        ../../solverinterface.h

        src/simulation.h
        src/simulationsupportplugin.h
        src/simulationworker.h
    PLUGINS
        COMBINESupport
)
