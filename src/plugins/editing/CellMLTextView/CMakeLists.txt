PROJECT(CellMLTextViewPlugin)

# Retrieve the CellML API external binaries

RETRIEVE_CELLML_API_EXTERNAL_BINARIES()

# Apply LLVM's and Qwt's build settings

APPLY_LLVM_BUILD_SETTINGS()
APPLY_QWT_BUILD_SETTINGS()

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
    DEFINITIONS
        ${LLVM_DEFINITIONS}
        ${QWT_DEFINITIONS}
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
        conversiontests
        parsingtests
        scanningtests
)
