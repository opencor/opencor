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
    PLUGINS
        SUNDIALS
    QT_MODULES
        Core
)
