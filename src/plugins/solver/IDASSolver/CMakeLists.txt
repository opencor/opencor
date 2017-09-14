PROJECT(IDASSolverPlugin)

# Add the plugin

ADD_PLUGIN(IDASSolver
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../solverinterface.cpp

        src/idassolver.cpp
        src/idassolverplugin.cpp
    HEADERS_MOC
        ../../solverinterface.h

        src/idassolver.h
        src/idassolverplugin.h
    PLUGINS
        SUNDIALS
    QT_MODULES
        Widgets
)
