project(EditorWidgetPlugin)

# Add the plugin

add_plugin(EditorWidget
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/editorlistwidget.cpp
        src/editorwidget.cpp
        src/editorwidgetfindreplacewidget.cpp
        src/editorwidgetplugin.cpp
    HEADERS_MOC
        src/editorlistwidget.h
        src/editorwidget.h
        src/editorwidgetfindreplacewidget.h
        src/editorwidgetplugin.h
    UIS
        src/editorwidgetfindreplacewidget.ui
    PLUGINS
        QScintillaSupport
)
