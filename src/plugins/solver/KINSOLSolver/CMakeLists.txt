PROJECT(KINSOLSolverPlugin)

# Add the plugin

ADD_PLUGIN(KINSOLSolver
    SOURCES
        ../../plugininfo.cpp
        ../../solverinterface.cpp

        src/kinsolsolver.cpp
        src/kinsolsolverplugin.cpp
    HEADERS_MOC
        ../../solverinterface.h

        src/kinsolsolverplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        ${SUNDIALS_PLUGIN}
    PLUGIN_BINARIES
        ${SUNDIALS_PLUGIN_BINARY}
    QT_MODULES
        Core
)
