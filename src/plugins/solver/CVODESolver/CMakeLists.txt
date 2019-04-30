project(CVODESolverPlugin)

# Add the plugin

add_plugin(CVODESolver
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../solverinterface.cpp

        src/cvodesolver.cpp
        src/cvodesolverplugin.cpp
    PLUGINS
        SUNDIALS
    QT_MODULES
        Widgets
)
