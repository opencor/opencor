PROJECT(PrettyCellMLViewPlugin)

# Retrieve some CellML API and LLVM settings

RETRIEVE_CELLML_API_SETTINGS()
RETRIEVE_LLVM_SETTINGS()

# Add the plugin

IF(APPLE)
    SET(QT_MAC_EXTRAS QtMacExtras)
ELSE()
    SET(QT_MAC_EXTRAS)
ENDIF()

ADD_PLUGIN(PrettyCellMLView
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../viewinterface.cpp

        src/prettycellmlviewcellmltoprettycellmlconverter.cpp
#---GRY---
#        src/prettycellmlviewlexer.cpp
        src/prettycellmlviewplugin.cpp
        src/prettycellmlviewwidget.cpp
    HEADERS_MOC
#---GRY---
#        src/prettycellmlviewlexer.h
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
        CoreCellMLEditing
        CoreEditing
        CoreSolver
        Editor
        EditorList
        ${LLVM_PLUGIN}
        ${QSCINTILLA_PLUGIN}
        QScintillaSupport
        ${QWT_PLUGIN}
        Viewer
    PLUGIN_BINARIES
        ${LLVM_PLUGIN_BINARY}
        ${QSCINTILLA_PLUGIN_BINARY}
        ${QWT_PLUGIN_BINARY}
    QT_LIBRARIES
        QtConcurrent
        QtCore
        QtGui
        ${QT_MAC_EXTRAS}
        QtNetwork
        QtOpenGL
        QtPrintSupport
        QtSvg
        QtWidgets
        QtXml
        QtXmlPatterns
    EXTERNAL_BINARIES
        ${CELLML_API_EXTERNAL_BINARIES}
)
