PROJECT(PrettyCellMLViewPlugin)

# Retrieve some CellML API and LLVM settings

RETRIEVE_CELLML_API_SETTINGS()
RETRIEVE_LLVM_SETTINGS()

# Add the plugin

ADD_PLUGIN(PrettyCellMLView
    SOURCES
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/prettycellmlviewplugin.cpp
        src/prettycellmlviewwidget.cpp
    HEADERS_MOC
        src/prettycellmlviewplugin.h
        src/prettycellmlviewwidget.h
    UIS
        src/prettycellmlviewwidget.ui
    DEFINITIONS
        ${LLVM_DEFINITIONS}
    INCLUDE_DIRS
        src
    PLUGINS
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
    PLUGIN_BINARIES
        ${LLVM_BINARY_PLUGIN}
    QT_MODULES
        Widgets
    QT_LIBRARIES
        QtConcurrent
        QtCore
        QtGui
        QtNetwork
        QtOpenGL
        QtPrintSupport
        QtSvg
        QtWidgets
        QtXml
    EXTERNAL_BINARIES
        ${CELLML_API_EXTERNAL_BINARIES}
)
