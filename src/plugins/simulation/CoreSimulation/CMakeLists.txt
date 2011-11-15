PROJECT(CoreSimulationPlugin)

# Retrieve LLVM settings

RETRIEVE_LLVM_SETTINGS()

# Add the plugin

ADD_PLUGIN(CoreSimulation
    SOURCES
        ../../coreinterface.cpp
        ../../guiinterface.cpp
        ../../plugininfo.cpp

        src/coresimulationplugin.cpp
    HEADERS
        ../../coreinterface.h
        ../../guiinterface.h
        ../../plugininfo.h
    HEADERS_MOC
        src/coresimulationplugin.h
    DEFINITIONS
        ${LLVM_DEFINITIONS}
    INCLUDE_DIRS
        ../../3rdparty/Qwt/src

        src
    OPENCOR_DEPENDENCIES
        Core
        LLVM
        Qwt
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtSvg
        QtXml
    EXTERNAL_DEPENDENCIES
        ${LLVM_EXTERNAL_DEPENDENCIES}
)
