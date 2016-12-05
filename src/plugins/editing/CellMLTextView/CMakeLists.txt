PROJECT(CellMLTextViewPlugin)

# Add the plugin

ADD_PLUGIN(CellMLTextView
    SOURCES
        ../../cliinterface.cpp
        ../../filehandlinginterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../viewinterface.cpp

        ../CellMLEditingView/src/cellmleditingviewinterface.cpp
        ../EditingView/src/editingviewinterface.cpp

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
        CellMLEditingView
        EditingView
        EditorWidget
        ${LLVM_PLUGIN}
        MathMLViewerWidget
        ${QSCINTILLA_PLUGIN}
        QScintillaSupport
        ${QWT_PLUGIN}
    PLUGIN_BINARIES
        ${LLVM_PLUGIN_BINARY}
        ${QSCINTILLA_PLUGIN_BINARY}
        ${QWT_PLUGIN_BINARY}
    TESTS
        clitests
        conversiontests
        parsingtests
        scanningtests
)
