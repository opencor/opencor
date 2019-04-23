project(KINSOLSolverPlugin)

# Add the plugin

add_plugin(KINSOLSolver
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../solverinterface.cpp

        src/kinsolsolver.cpp
        src/kinsolsolverplugin.cpp
    PLUGINS
        SUNDIALS
    QT_MODULES
        Widgets
)
