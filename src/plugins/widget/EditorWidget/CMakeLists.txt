PROJECT(EditorWidgetPlugin)

# Add the plugin

ADD_PLUGIN(EditorWidget
    SOURCES
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/editorfindreplacewidget.cpp
        src/editorlistitem.cpp
        src/editorlistwidget.cpp
        src/editorwidget.cpp
        src/editorwidgetplugin.cpp
    HEADERS_MOC
        src/editorlistwidget.h
        src/editorfindreplacewidget.h
        src/editorwidget.h
        src/editorwidgetplugin.h
    UIS
        src/editorfindreplacewidget.ui
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        ${QSCINTILLA_PLUGIN}
        QScintillaSupport
    PLUGIN_BINARIES
        ${QSCINTILLA_PLUGIN_BINARY}
)
