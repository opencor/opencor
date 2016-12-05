PROJECT(RawSEDMLViewPlugin)

# Add the plugin

ADD_PLUGIN(RawSEDMLView
    SOURCES
        ../../filehandlinginterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../viewinterface.cpp

        ../EditingView/src/editingviewinterface.cpp
        ../SEDMLEditingView/src/sedmleditingviewinterface.cpp

        src/rawsedmlviewplugin.cpp
        src/rawsedmlviewwidget.cpp
    HEADERS_MOC
        src/rawsedmlviewplugin.h
        src/rawsedmlviewwidget.h
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
        SEDMLEditingView
        SEDMLSupport
    PLUGIN_BINARIES
        ${LLVM_PLUGIN_BINARY}
        ${QSCINTILLA_PLUGIN_BINARY}
)
