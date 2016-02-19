PROJECT(CoreSEDMLEditingPlugin)

# Retrieve the CellML, SBML and SED-ML APIs' external binaries

RETRIEVE_CELLML_API_EXTERNAL_BINARIES()
RETRIEVE_SBML_API_EXTERNAL_BINARIES()
RETRIEVE_SEDML_API_EXTERNAL_BINARIES()

# Apply LLVM's build settings

APPLY_LLVM_BUILD_SETTINGS()

# Add the plugin

ADD_PLUGIN(CoreSEDMLEditing
    SOURCES
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../pluginmanager.cpp

        src/coresedmleditingplugin.cpp
        src/coresedmleditingwidget.cpp
    HEADERS_MOC
        ../../plugin.h
        ../../pluginmanager.h

        src/coresedmleditingplugin.h
        src/coresedmleditingwidget.h
    INCLUDE_DIRS
        src
    DEFINITIONS
        ${LLVM_DEFINITIONS}
    PLUGINS
        CellMLAPI
        CellMLSupport
        Core
        CoreEditing
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
