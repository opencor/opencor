PROJECT(RawCellMLViewPlugin)

# Retrieve some CellML API and LLVM settings

RETRIEVE_CELLML_API_SETTINGS()
RETRIEVE_LLVM_SETTINGS()

# Add the plugin

ADD_PLUGIN(RawCellMLView
    SOURCES
        ../../coreinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../interface.cpp
        ../../plugininfo.cpp

        src/rawcellmlviewplugin.cpp
        src/rawcellmlviewwidget.cpp
    HEADERS_MOC
        src/rawcellmlviewplugin.h
        src/rawcellmlviewwidget.h
    UIS
        src/rawcellmlviewwidget.ui
    DEFINITIONS
        ${LLVM_DEFINITIONS}
    INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        CellMLAPI
        CellMLSupport
        Computer
        Core
        ${LLVM_PLUGIN}
        QScintilla
        QScintillaSupport
        QtMmlWidget
        Viewer
    OPENCOR_BINARY_DEPENDENCIES
        ${LLVM_BINARY_PLUGIN}
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtNetwork
        QtXml
    EXTERNAL_DEPENDENCIES
        ${CELLML_API_EXTERNAL_DEPENDENCIES}
)
