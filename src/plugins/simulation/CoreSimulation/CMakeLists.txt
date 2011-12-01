PROJECT(CoreSimulationPlugin)

# Retrieve some CellML and LLVM settings

RETRIEVE_CELLML_SETTINGS()
RETRIEVE_LLVM_SETTINGS()

# Add the plugin

ADD_PLUGIN(CoreSimulation
    SOURCES
        ../../coreinterface.cpp
        ../../guiinterface.cpp
        ../../plugininfo.cpp

        src/coresimulationplugin.cpp
    HEADERS_MOC
        src/coresimulationplugin.h
    DEFINITIONS
        ${CELLML_DEFINITIONS}
        ${LLVM_DEFINITIONS}
    INCLUDE_DIRS
        ../../3rdparty/Qwt/src

        src
    OPENCOR_DEPENDENCIES
        CellML
        CellMLModel
        Core
        CVODE
        IDA
        LLVM
        Qwt
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtSvg
        QtXml
    EXTERNAL_DEPENDENCIES
        ${CELLML_EXTERNAL_DEPENDENCIES}
        ${LLVM_EXTERNAL_DEPENDENCIES}
)
