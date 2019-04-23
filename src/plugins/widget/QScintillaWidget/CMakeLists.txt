project(QScintillaWidgetPlugin)

# Add the plugin

add_plugin(QScintillaWidget
    SOURCES
        ../../plugininfo.cpp

        src/qscintillawidgetplugin.cpp
        src/qscintillawidget.cpp
    PLUGINS
        Core
        QScintilla
)
