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
        src/idasolverplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        CoreSolver
        ${SUNDIALS_PLUGIN}
    PLUGIN_BINARIES
        ${SUNDIALS_PLUGIN_BINARY}
    QT_MODULES
        Widgets
    QT_LIBRARIES
        QtCore
        QtGui
        QtWidgets
)
