PROJECT(RawSEDMLViewPlugin)

# Retrieve the SBML and SED-ML APIs' external binaries

RETRIEVE_SBML_API_EXTERNAL_BINARIES()
RETRIEVE_SEDML_API_EXTERNAL_BINARIES()

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
    PLUGINS
        Core
        CoreEditing
        CoreSEDMLEditing
        Editor
        EditorList
        ${QSCINTILLA_PLUGIN}
        QScintillaSupport
        SBMLAPI
        SEDMLAPI
        SEDMLSupport
    PLUGIN_BINARIES
        ${QSCINTILLA_PLUGIN_BINARY}
    EXTERNAL_BINARIES
        ${SBML_API_EXTERNAL_BINARIES}
        ${SEDML_API_EXTERNAL_BINARIES}
)
