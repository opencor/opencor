PROJECT(CoreCellMLEditingPlugin)

# Retrieve some CellML API and LLVM settings

RETRIEVE_CELLML_API_SETTINGS()
RETRIEVE_LLVM_SETTINGS()

# Add the plugin

IF(APPLE)
    SET(QT_MAC_EXTRAS QtMacExtras)
ELSE()
    SET(QT_MAC_EXTRAS)
ENDIF()

ADD_PLUGIN(CoreCellMLEditing
    SOURCES
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../pluginmanager.cpp

        src/corecellmleditingplugin.cpp
        src/corecellmleditingwidget.cpp
    HEADERS_MOC
        ../../plugin.h
        ../../pluginmanager.h

        src/corecellmleditingplugin.h
        src/corecellmleditingwidget.h
    UIS
        src/corecellmleditingwidget.ui
    INCLUDE_DIRS
        src
    DEFINITIONS
        ${LLVM_DEFINITIONS}
    PLUGINS
        CellMLAPI
        CellMLSupport
        Compiler
        Core
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
