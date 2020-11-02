project(EditorWidgetPlugin)

# Add the plugin

add_plugin(EditorWidget
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/editorlistwidget.cpp
        src/editorwidget.cpp
        src/editorwidgeteditorwidget.cpp
        src/editorwidgetfindreplacewidget.cpp
        src/editorwidgetplugin.cpp
        src/editorwidgetscrollbar.cpp
    UIS
        src/editorwidgetfindreplacewidget.ui
    PLUGINS
        QScintillaWidget
)
