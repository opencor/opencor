PROJECT(SimulationSupportPlugin)

# Add the plugin

ADD_PLUGIN(SimulationSupport
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
    HEADERS_MOC
        ../../datastoreinterface.h
        ../../solverinterface.h

        src/simulation.h
        src/simulationmanager.h
        src/simulationsupportplugin.h
        src/simulationsupportpythonwrapper.h
        src/simulationworker.h
    PLUGINS
        COMBINESupport
        DataStore
        PythonQtSupport
)
