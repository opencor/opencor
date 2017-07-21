PROJECT(SimulationSupportPlugin)

# Add the plugin

ADD_PLUGIN(SimulationSupport
    SOURCES
        src/simulation.cpp
        src/simulationsupportplugin.cpp
        src/simulationworker.cpp
    HEADERS_MOC
        src/simulation.h
        src/simulationsupportplugin.h
        src/simulationworker.h
    PLUGINS
        COMBINESupport
)
