PROJECT(ForwardEulerSolverPlugin)

# Add the plugin

ADD_PLUGIN(ForwardEulerSolver
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../solverinterface.cpp

        src/forwardeulersolver.cpp
        src/forwardeulersolverplugin.cpp
    HEADERS_MOC
        ../../solverinterface.h

        src/forwardeulersolverplugin.h
    QT_MODULES
        Widgets
)
