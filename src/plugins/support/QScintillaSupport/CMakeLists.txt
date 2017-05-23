PROJECT(QScintillaSupportPlugin)

# Add the plugin

ADD_PLUGIN(QScintillaSupport
    SOURCES
        ../../plugininfo.cpp

        src/qscintillasupportplugin.cpp
        src/qscintillawidget.cpp
    HEADERS_MOC
        src/qscintillasupportplugin.h
        src/qscintillawidget.h
    DEFINITIONS
        QSCINTILLA_DLL
    PLUGINS
        Core
        QScintilla
)
