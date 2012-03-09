PROJECT(CVODESolverPlugin)

# Add the plugin

ADD_PLUGIN(CVODESolver
    SOURCES
        ../../interface.cpp
        ../../plugininfo.cpp
        ../../solverinterface.cpp

        src/cvodesolver.cpp
        src/cvodesolverplugin.cpp
    HEADERS_MOC
        src/cvodesolverplugin.h
    INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        CoreSolver
        SUNDIALS
    QT_DEPENDENCIES
        QtCore
)
