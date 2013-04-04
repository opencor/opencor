PROJECT(HeunSolverPlugin)

# Add the plugin

ADD_PLUGIN(HeunSolver
    SOURCES
        ../../i18ninterface.cpp
        ../../interface.cpp
        ../../plugininfo.cpp
        ../../solverinterface.cpp

        src/heunsolver.cpp
        src/heunsolverplugin.cpp
    HEADERS_MOC
        src/heunsolverplugin.h
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
