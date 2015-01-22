PROJECT(EditorPlugin)

# Add the plugin

ADD_PLUGIN(Editor
    SOURCES
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/editorfindreplacewidget.cpp
        src/editorplugin.cpp
        src/editorwidget.cpp
    HEADERS_MOC
        src/editorfindreplacewidget.h
        src/editorplugin.h
        src/editorwidget.h
    UIS
        src/editorfindreplacewidget.ui
        src/editorwidget.ui
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        ${QSCINTILLA_PLUGIN}
        QScintillaSupport
    PLUGIN_BINARIES
        ${QSCINTILLA_PLUGIN_BINARY}
    QT_LIBRARIES
        QtCore
        QtGui
        QtNetwork
        QtWidgets
        QtXml
        QtXmlPatterns
)
