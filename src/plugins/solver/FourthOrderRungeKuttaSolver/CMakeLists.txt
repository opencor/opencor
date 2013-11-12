PROJECT(FourthOrderRungeKuttaSolverPlugin)

# Add the plugin

ADD_PLUGIN(FourthOrderRungeKuttaSolver
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../solverinterface.cpp

        src/fourthorderrungekuttasolver.cpp
        src/fourthorderrungekuttasolverplugin.cpp
    HEADERS_MOC
        src/fourthorderrungekuttasolverplugin.h
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
