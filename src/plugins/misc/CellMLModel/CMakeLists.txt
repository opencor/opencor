PROJECT(CellMLModelPlugin)

# Retrieve some CellML and LLVM settings

RETRIEVE_CELLML_SETTINGS()
RETRIEVE_LLVM_SETTINGS()

# Add the plugin

ADD_PLUGIN(CellMLModel
    SOURCES
        ../../plugininfo.cpp

        src/cellmlmodelplugin.cpp
    HEADERS_MOC
        src/cellmlmodelplugin.h
    DEFINITIONS
        ${CELLML_DEFINITIONS}
        ${LLVM_DEFINITIONS}
    INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        CellML
        Core
        LLVM
    QT_DEPENDENCIES
        QtCore
        QtGui
    EXTERNAL_DEPENDENCIES
        ${CELLML_EXTERNAL_DEPENDENCIES}
        ${LLVM_EXTERNAL_DEPENDENCIES}
)
