PROJECT(RawCellMLViewPlugin)

# Retrieve LLVM's definitions

RETRIEVE_LLVM_DEFINITIONS()

# Add the plugin

ADD_PLUGIN(RawCellMLView
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../viewinterface.cpp

        src/rawcellmlviewplugin.cpp
        src/rawcellmlviewwidget.cpp
    HEADERS_MOC
        src/rawcellmlviewplugin.h
        src/rawcellmlviewwidget.h
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
