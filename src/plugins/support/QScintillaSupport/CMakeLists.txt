project(QScintillaSupportPlugin)

# Add the plugin

add_plugin(QScintillaSupport
    SOURCES
        ../../plugininfo.cpp

        src/qscintillasupportplugin.cpp
        src/qscintillawidget.cpp
    HEADERS_MOC
        src/qscintillasupportplugin.h
        src/qscintillawidget.h
    PLUGINS
        Core
        QScintilla
)
