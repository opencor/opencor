PROJECT(RawCellMLViewPlugin)

# Retrieve some CellML API and LLVM settings

RETRIEVE_CELLML_API_SETTINGS()
RETRIEVE_LLVM_SETTINGS()

# Add the plugin

ADD_PLUGIN(RawCellMLView
    SOURCES
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
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
    PLUGIN_DEPENDENCIES
        CellMLAPI
        CellMLSupport
        Compiler
        Core
        CoreSolver
        ${LLVM_PLUGIN}
        QScintilla
        QScintillaSupport
        Qwt
        Viewer
    PLUGIN_BINARY_DEPENDENCIES
        ${LLVM_BINARY_PLUGIN}
    QT_MODULES
        Widgets
    QT_DEPENDENCIES
        QtConcurrent
        QtCore
        QtGui
        QtNetwork
        QtOpenGL
        QtPrintSupport
        QtSvg
        QtWidgets
        QtXml
    EXTERNAL_BINARY_DEPENDENCIES
        ${CELLML_API_EXTERNAL_BINARY_DEPENDENCIES}
)
