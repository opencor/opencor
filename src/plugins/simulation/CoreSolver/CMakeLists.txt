PROJECT(CoreSolverPlugin)

# Add the plugin

ADD_PLUGIN(CoreSolver
    SOURCES
        ../../plugininfo.cpp

        src/coreodesolver.cpp
        src/coresolverplugin.cpp
    HEADERS_MOC
        src/coresolverplugin.h
    INCLUDE_DIRS
        src
    QT_DEPENDENCIES
        QtCore
)
