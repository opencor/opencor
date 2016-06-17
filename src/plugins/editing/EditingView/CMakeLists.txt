PROJECT(EditingViewPlugin)

# Add the plugin

ADD_PLUGIN(EditingView
    SOURCES
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../pluginmanager.cpp

        src/editingviewplugin.cpp
    HEADERS_MOC
        ../../plugin.h
        ../../pluginmanager.h

        src/editingviewplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        EditorWidget
        ${QSCINTILLA_PLUGIN}
        QScintillaSupport
    PLUGIN_BINARIES
        ${QSCINTILLA_PLUGIN_BINARY}
)
