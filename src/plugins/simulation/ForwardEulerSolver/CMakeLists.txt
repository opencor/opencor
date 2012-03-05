PROJECT(ForwardEulerSolverPlugin)

# Add the plugin

ADD_PLUGIN(ForwardEulerSolver
    SOURCES
        ../../plugininfo.cpp

        src/forwardeulersolverplugin.cpp
    HEADERS_MOC
        src/forwardeulersolverplugin.h
    INCLUDE_DIRS
        src
    QT_DEPENDENCIES
        QtCore
)
