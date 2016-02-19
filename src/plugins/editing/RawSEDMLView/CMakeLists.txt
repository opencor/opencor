PROJECT(RawSEDMLViewPlugin)

# Retrieve the CellML, SBML and SED-ML APIs' external binaries

RETRIEVE_CELLML_API_EXTERNAL_BINARIES()
RETRIEVE_SBML_API_EXTERNAL_BINARIES()
RETRIEVE_SEDML_API_EXTERNAL_BINARIES()

# Apply LLVM's build settings

APPLY_LLVM_BUILD_SETTINGS()

# Add the plugin

ADD_PLUGIN(RawSEDMLView
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../viewinterface.cpp

        src/rawsedmlviewplugin.cpp
        src/rawsedmlviewwidget.cpp
    HEADERS_MOC
        src/rawsedmlviewplugin.h
        src/rawsedmlviewwidget.h
    INCLUDE_DIRS
        src
    DEFINITIONS
        ${LLVM_DEFINITIONS}
    PLUGINS
        CellMLAPI
        CellMLSupport
        Core
        CoreEditing
        CoreSEDMLEditing
        Editor
        EditorList
        ${LLVM_PLUGIN}
        ${QSCINTILLA_PLUGIN}
        QScintillaSupport
        SBMLAPI
        SEDMLAPI
        SEDMLSupport
    PLUGIN_BINARIES
        ${LLVM_PLUGIN_BINARY}
        ${QSCINTILLA_PLUGIN_BINARY}
    EXTERNAL_BINARIES
        ${CELLML_API_EXTERNAL_BINARIES}
        ${SBML_API_EXTERNAL_BINARIES}
        ${SEDML_API_EXTERNAL_BINARIES}
)
