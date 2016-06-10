PROJECT(CellMLTextViewPlugin)

# Add the plugin

ADD_PLUGIN(CellMLTextView
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../viewinterface.cpp

        src/cellmltextviewconverter.cpp
        src/cellmltextviewlexer.cpp
        src/cellmltextviewparser.cpp
        src/cellmltextviewplugin.cpp
        src/cellmltextviewscanner.cpp
        src/cellmltextviewwidget.cpp
    HEADERS_MOC
        src/cellmltextviewplugin.h
        src/cellmltextviewwidget.h
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
    TESTS
        clitests
        conversiontests
        parsingtests
        scanningtests
)
