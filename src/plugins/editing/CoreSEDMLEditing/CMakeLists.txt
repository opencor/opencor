PROJECT(CoreSEDMLEditingPlugin)

# Retrieve the SBML and SED-ML APIs' external binaries

RETRIEVE_SBML_API_EXTERNAL_BINARIES()
RETRIEVE_SEDML_API_EXTERNAL_BINARIES()

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
    PLUGINS
        Core
        CoreEditing
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
