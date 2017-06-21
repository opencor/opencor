PROJECT(IDASolverPlugin)

# Add the plugin

ADD_PLUGIN(IDASolver
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../solverinterface.cpp

        src/idasolver.cpp
        src/idasolverplugin.cpp
    HEADERS_MOC
        ../../solverinterface.h

        src/idasolverplugin.h
    PLUGINS
        SUNDIALS
    QT_MODULES
        Widgets
)
