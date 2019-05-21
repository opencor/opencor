project(SolverSupportPlugin)

# Add the plugin

add_plugin(SolverSupport
    SOURCES
        ../../plugininfo.cpp
        ../../solverinterface.cpp

        src/solversupportplugin.cpp
    PLUGINS
        COMBINESupport
)
