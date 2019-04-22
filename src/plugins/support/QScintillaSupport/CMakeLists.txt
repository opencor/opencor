project(QScintillaSupportPlugin)

# Add the plugin

add_plugin(QScintillaSupport
    SOURCES
        ../../plugininfo.cpp

        src/qscintillasupportplugin.cpp
        src/qscintillawidget.cpp
    PLUGINS
        Core
        QScintilla
)
