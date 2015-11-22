PROJECT(CoreSEDMLEditingPlugin)

# Retrieve the SED-ML API external binaries

RETRIEVE_SEDML_API_EXTERNAL_BINARIES()

# Apply LLVM's and Qwt's build settings

APPLY_LLVM_BUILD_SETTINGS()
APPLY_QWT_BUILD_SETTINGS()

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
        ${QWT_DEFINITIONS}
    PLUGINS
        CellMLSupport
        Compiler
        Core
        CoreEditing
        Editor
        EditorList
        ${LLVM_PLUGIN}
        ${QSCINTILLA_PLUGIN}
        QScintillaSupport
        ${QWT_PLUGIN}
        SEDMLAPI
        Viewer
    PLUGIN_BINARIES
        ${LLVM_PLUGIN_BINARY}
        ${QSCINTILLA_PLUGIN_BINARY}
        ${QWT_PLUGIN_BINARY}
    EXTERNAL_BINARIES
        ${SEDML_API_EXTERNAL_BINARIES}
)
