PROJECT(RawSEDMLViewPlugin)

# Retrieve the SED-ML API external binaries

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
        SEDMLAPI
        SEDMLSupport
    PLUGIN_BINARIES
        ${QSCINTILLA_PLUGIN_BINARY}
    EXTERNAL_BINARIES
        ${SEDML_API_EXTERNAL_BINARIES}
)
