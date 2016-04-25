PROJECT(RawCellMLViewPlugin)

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
