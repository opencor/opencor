PROJECT(KINSOLSolverPlugin)

# Add the plugin

ADD_PLUGIN(KINSOLSolver
    SOURCES
        ../../plugininfo.cpp
        ../../solverinterface.cpp

        src/kinsolsolver.cpp
        src/kinsolsolverplugin.cpp
    HEADERS_MOC
        src/kinsolsolverplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        CoreSolver
        ${SUNDIALS_PLUGIN}
    PLUGIN_BINARIES
        ${SUNDIALS_PLUGIN_BINARY}
    QT_MODULES
        Core
    QT_LIBRARIES
        QtCore
)
