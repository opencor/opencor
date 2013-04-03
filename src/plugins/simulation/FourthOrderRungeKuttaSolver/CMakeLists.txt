PROJECT(FourthOrderRungeKuttaSolverPlugin)

# Add the plugin

ADD_PLUGIN(FourthOrderRungeKuttaSolver
    SOURCES
        ../../i18ninterface.cpp
        ../../interface.cpp
        ../../plugininfo.cpp
        ../../solverinterface.cpp

        src/fourthorderrungekuttasolver.cpp
        src/fourthorderrungekuttasolverplugin.cpp
    HEADERS_MOC
        src/fourthorderrungekuttasolverplugin.h
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
