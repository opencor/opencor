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
        ../../solverinterface.h

        src/heunsolverplugin.h
    QT_MODULES
        Widgets
)
