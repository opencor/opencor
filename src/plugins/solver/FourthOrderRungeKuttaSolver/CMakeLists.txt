project(FourthOrderRungeKuttaSolverPlugin)

# Add the plugin

add_plugin(FourthOrderRungeKuttaSolver
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../solverinterface.cpp

        src/fourthorderrungekuttasolver.cpp
        src/fourthorderrungekuttasolverplugin.cpp
    HEADERS_MOC
        ../../solverinterface.h

        src/fourthorderrungekuttasolver.h
        src/fourthorderrungekuttasolverplugin.h
    QT_MODULES
        Widgets
)
