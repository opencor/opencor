PROJECT(CoreCellMLEditingPlugin)

# Retrieve LLVM's definitions

RETRIEVE_LLVM_DEFINITIONS()

# Add the plugin

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
        Editor
        EditorList
        ${LLVM_PLUGIN}
        MathMLViewer
        ${QSCINTILLA_PLUGIN}
        QScintillaSupport
        ${QWT_PLUGIN}
    PLUGIN_BINARIES
        ${LLVM_PLUGIN_BINARY}
        ${QSCINTILLA_PLUGIN_BINARY}
        ${QWT_PLUGIN_BINARY}
    EXTERNAL_BINARIES
        ${CELLML_API_EXTERNAL_BINARIES}
)
