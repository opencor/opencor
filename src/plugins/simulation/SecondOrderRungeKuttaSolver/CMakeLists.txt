PROJECT(SecondOrderRungeKuttaSolverPlugin)

# Add the plugin

ADD_PLUGIN(SecondOrderRungeKuttaSolver
    SOURCES
        ../../i18ninterface.cpp
        ../../interface.cpp
        ../../plugininfo.cpp
        ../../solverinterface.cpp

        src/secondorderrungekuttasolver.cpp
        src/secondorderrungekuttasolverplugin.cpp
    HEADERS_MOC
        src/secondorderrungekuttasolverplugin.h
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
