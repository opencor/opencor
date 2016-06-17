PROJECT(RawTextViewPlugin)

# Add the plugin

ADD_PLUGIN(RawTextView
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../viewinterface.cpp

        src/rawtextviewplugin.cpp
        src/rawtextviewwidget.cpp
    HEADERS_MOC
        src/rawtextviewplugin.h
        src/rawtextviewwidget.h
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
