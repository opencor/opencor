PROJECT(EditorListPlugin)

# Add the plugin

ADD_PLUGIN(EditorList
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/editorlistitem.cpp
        src/editorlistplugin.cpp
        src/editorlistwidget.cpp
    HEADERS_MOC
        src/editorlistplugin.h
        src/editorlistwidget.h
    INCLUDE_DIRS
        src
    PLUGINS
        Core
    QT_LIBRARIES
        QtCore
        QtGui
        QtNetwork
        QtWidgets
        QtXml
        QtXmlPatterns
)
