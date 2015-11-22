PROJECT(RawSEDMLViewPlugin)

# Retrieve the SED-ML API external binaries

RETRIEVE_SEDML_API_EXTERNAL_BINARIES()

# Apply LLVM's and Qwt's build settings

APPLY_LLVM_BUILD_SETTINGS()
APPLY_QWT_BUILD_SETTINGS()

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
    DEFINITIONS
        ${LLVM_DEFINITIONS}
        ${QWT_DEFINITIONS}
    INCLUDE_DIRS
        src
    PLUGINS
        Compiler
        Core
        CoreEditing
        CoreSEDMLEditing
        Editor
        EditorList
        ${LLVM_PLUGIN}
        ${QSCINTILLA_PLUGIN}
        QScintillaSupport
        ${QWT_PLUGIN}
        SEDMLAPI
        SEDMLSupport
        Viewer
    PLUGIN_BINARIES
        ${LLVM_PLUGIN_BINARY}
        ${QSCINTILLA_PLUGIN_BINARY}
        ${QWT_PLUGIN_BINARY}
    EXTERNAL_BINARIES
        ${SEDML_API_EXTERNAL_BINARIES}
)
