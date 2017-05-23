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
        ../../solverinterface.h

        src/fourthorderrungekuttasolverplugin.h
    QT_MODULES
        Widgets
)
