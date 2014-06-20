PROJECT(EditorListPlugin)

# Add the plugin

ADD_PLUGIN(EditorList
    SOURCES
        ../../plugininfo.cpp

        src/editorlistwidget.cpp
        src/editorlistplugin.cpp
    HEADERS_MOC
        src/editorlistwidget.h
        src/editorlistplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        Core
    QT_MODULES
        Network
        Widgets
    QT_LIBRARIES
        QtCore
        QtGui
        QtNetwork
        QtWidgets
        QtXmlPatterns
)
