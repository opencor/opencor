PROJECT(IDASolverPlugin)

# Add the plugin

ADD_PLUGIN(IDASolver
    SOURCES
        ../../i18ninterface.cpp
        ../../interface.cpp
        ../../plugininfo.cpp
        ../../solverinterface.cpp

        src/idasolver.cpp
        src/idasolverplugin.cpp
    HEADERS_MOC
        src/idasolverplugin.h
    INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        CoreSolver
        SUNDIALS
    QT_MODULES
        Core
        Widgets
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtWidgets
)
