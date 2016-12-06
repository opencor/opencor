PROJECT(CellMLEditingViewPlugin)

# Add the plugin

ADD_PLUGIN(CellMLEditingView
    SOURCES
        ../../filehandlinginterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../pluginmanager.cpp

        src/cellmleditingviewinterface.cpp
        src/cellmleditingviewplugin.cpp
        src/cellmleditingviewwidget.cpp
    HEADERS_MOC
        ../../plugin.h
        ../../pluginmanager.h

        src/cellmleditingviewplugin.h
        src/cellmleditingviewwidget.h
    INCLUDE_DIRS
        src
    PLUGINS
        CellMLAPI
        CellMLSupport
        Compiler
        Core
        EditingView
        EditorWidget
        ${LLVM_PLUGIN}
        MathMLViewerWidget
        ${QSCINTILLA_PLUGIN}
        QScintillaSupport
        ${QWT_PLUGIN}
    PLUGIN_BINARIES
        ${LLVM_PLUGIN_BINARY}
        ${QSCINTILLA_PLUGIN_BINARY}
        ${QWT_PLUGIN_BINARY}
)
