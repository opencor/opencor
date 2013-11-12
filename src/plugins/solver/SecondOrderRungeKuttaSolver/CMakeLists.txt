PROJECT(SecondOrderRungeKuttaSolverPlugin)

# Add the plugin

ADD_PLUGIN(SecondOrderRungeKuttaSolver
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../solverinterface.cpp

        src/secondorderrungekuttasolver.cpp
        src/secondorderrungekuttasolverplugin.cpp
    HEADERS_MOC
        src/secondorderrungekuttasolverplugin.h
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
