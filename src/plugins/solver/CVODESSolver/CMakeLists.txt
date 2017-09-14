PROJECT(CVODESSolverPlugin)

# Add the plugin

ADD_PLUGIN(CVODESSolver
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../solverinterface.cpp

        src/cvodessolver.cpp
        src/cvodessolverplugin.cpp
    HEADERS_MOC
        ../../solverinterface.h

        src/cvodessolver.h
        src/cvodessolverplugin.h
    PLUGINS
        SUNDIALS
    QT_MODULES
        Widgets
)
