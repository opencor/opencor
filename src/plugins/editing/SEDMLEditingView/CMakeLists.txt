PROJECT(SEDMLEditingViewPlugin)

# Add the plugin

ADD_PLUGIN(SEDMLEditingView
    SOURCES
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../pluginmanager.cpp

        src/sedmleditingviewplugin.cpp
        src/sedmleditingviewwidget.cpp
    HEADERS_MOC
        ../../plugin.h
        ../../pluginmanager.h

        src/sedmleditingviewplugin.h
        src/sedmleditingviewwidget.h
    INCLUDE_DIRS
        src
    PLUGINS
        CellMLAPI
        CellMLSupport
        Core
        EditingView
        EditorWidget
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
