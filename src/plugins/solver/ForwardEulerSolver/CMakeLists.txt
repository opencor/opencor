project(ForwardEulerSolverPlugin)

# Add the plugin

add_plugin(ForwardEulerSolver
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../solverinterface.cpp

        src/forwardeulersolver.cpp
        src/forwardeulersolverplugin.cpp
    HEADERS_MOC
        ../../solverinterface.h

        src/forwardeulersolver.h
        src/forwardeulersolverplugin.h
    QT_MODULES
        Widgets
)
