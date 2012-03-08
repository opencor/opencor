PROJECT(ForwardEulerSolverPlugin)

# Add the plugin

ADD_PLUGIN(ForwardEulerSolver
    SOURCES
        ../../interface.cpp
        ../../plugininfo.cpp
        ../../solverinterface.cpp

        src/forwardeulersolver.cpp
        src/forwardeulersolverplugin.cpp
    HEADERS_MOC
        src/forwardeulersolverplugin.h
    INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        CoreSolver
    QT_DEPENDENCIES
        QtCore
)
