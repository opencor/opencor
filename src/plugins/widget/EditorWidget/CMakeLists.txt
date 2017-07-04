PROJECT(EditorWidgetPlugin)

# Add the plugin

ADD_PLUGIN(EditorWidget
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
