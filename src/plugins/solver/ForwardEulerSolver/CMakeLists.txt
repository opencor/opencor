project(ForwardEulerSolverPlugin)

# Add the plugin

add_plugin(ForwardEulerSolver
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../solverinterface.cpp

        src/forwardeulersolver.cpp
        src/forwardeulersolverplugin.cpp
    QT_MODULES
        Widgets
)
