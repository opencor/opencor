project(SecondOrderRungeKuttaSolverPlugin)

# Add the plugin

add_plugin(SecondOrderRungeKuttaSolver
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../solverinterface.cpp

        src/secondorderrungekuttasolver.cpp
        src/secondorderrungekuttasolverplugin.cpp
    QT_MODULES
        Widgets
)
