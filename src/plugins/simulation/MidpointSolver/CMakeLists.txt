PROJECT(MidpointSolverPlugin)

# Add the plugin

ADD_PLUGIN(MidpointSolver
    SOURCES
        ../../i18ninterface.cpp
        ../../interface.cpp
        ../../plugininfo.cpp
        ../../solverinterface.cpp

        src/midpointsolver.cpp
        src/midpointsolverplugin.cpp
    HEADERS_MOC
        src/midpointsolverplugin.h
    INCLUDE_DIRS
        src
    PLUGIN_DEPENDENCIES
        CoreSolver
    QT_MODULES
        Core
        Widgets
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtWidgets
)
