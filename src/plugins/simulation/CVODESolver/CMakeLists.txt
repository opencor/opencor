PROJECT(CVODESolverPlugin)

# Add the plugin

ADD_PLUGIN(CVODESolver
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../solverinterface.cpp

        src/cvodesolver.cpp
        src/cvodesolverplugin.cpp
    HEADERS_MOC
        src/cvodesolverplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        CoreSolver
        SUNDIALS
    QT_MODULES
        Core
        Widgets
    QT_LIBRARIES
        QtCore
        QtGui
        QtWidgets
)
