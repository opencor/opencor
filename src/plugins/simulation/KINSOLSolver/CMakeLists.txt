PROJECT(KINSOLSolverPlugin)

# Add the plugin

ADD_PLUGIN(KINSOLSolver
    SOURCES
        ../../interface.cpp
        ../../plugininfo.cpp
        ../../solverinterface.cpp

        src/kinsolsolver.cpp
        src/kinsolsolverplugin.cpp
    HEADERS_MOC
        src/kinsolsolverplugin.h
    INCLUDE_DIRS
        src
    OPENCOR_PLUGIN_DEPENDENCIES
        CoreSolver
        SUNDIALS
    QT_MODULES
        Core
    QT_DEPENDENCIES
        QtCore
)
