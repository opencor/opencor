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
        ../../solverinterface.h

        src/secondorderrungekuttasolverplugin.h
    INCLUDE_DIRS
        src
    QT_MODULES
        Widgets
)
