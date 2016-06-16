PROJECT(CoreEditingViewPlugin)

# Add the plugin

ADD_PLUGIN(CoreEditingView
    SOURCES
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../pluginmanager.cpp

        src/coreeditingviewplugin.cpp
    HEADERS_MOC
        ../../plugin.h
        ../../pluginmanager.h

        src/coreeditingviewplugin.h
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
