PROJECT(SimulationSupportPlugin)

# Add the plugin

ADD_PLUGIN(SimulationSupport
    SOURCES
        src/simulationsupportplugin.cpp
    HEADERS_MOC
        src/simulationsupportplugin.h
    PLUGINS
        COMBINESupport
)
