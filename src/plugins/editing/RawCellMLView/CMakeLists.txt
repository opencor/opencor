PROJECT(RawCellMLViewPlugin)

# Retrieve some CellML and LLVM settings

RETRIEVE_CELLML_SETTINGS()
RETRIEVE_LLVM_SETTINGS()

# Add the plugin

ADD_PLUGIN(RawCellMLView
    SOURCES
        ../../coreinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/rawcellmlviewplugin.cpp
    HEADERS_MOC
        src/rawcellmlviewplugin.h
    DEFINITIONS
        ${LLVM_DEFINITIONS}
    INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        CellMLSupport
        QScintilla
    QT_DEPENDENCIES
        QtCore
        QtGui
    EXTERNAL_DEPENDENCIES
        ${CELLML_EXTERNAL_DEPENDENCIES}
        ${LLVM_EXTERNAL_DEPENDENCIES}
)
