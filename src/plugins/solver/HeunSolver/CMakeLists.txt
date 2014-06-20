PROJECT(HeunSolverPlugin)

# Add the plugin

ADD_PLUGIN(HeunSolver
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../solverinterface.cpp

        src/heunsolver.cpp
        src/heunsolverplugin.cpp
    HEADERS_MOC
        src/heunsolverplugin.h
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
