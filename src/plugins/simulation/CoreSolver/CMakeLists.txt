PROJECT(CoreSolverPlugin)

# Add the plugin

ADD_PLUGIN(CoreSolver
    SOURCES
        ../../plugininfo.cpp

        src/coredaesolver.cpp
        src/corenlasolver.cpp
        src/coreodesolver.cpp
        src/coresolver.cpp
        src/coresolverplugin.cpp
    HEADERS_MOC
        src/coresolver.h
        src/coresolverplugin.h
    INCLUDE_DIRS
        src
    QT_MODULES
        Core
    QT_DEPENDENCIES
        QtCore
)
