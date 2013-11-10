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
        src/forwardeulersolverplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        CoreSolver
    QT_MODULES
        Core
        Widgets
    QT_LIBRARIES
        QtCore
        QtGui
        QtWidgets
)
