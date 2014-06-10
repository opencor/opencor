PROJECT(EditorListPlugin)

# Add the plugin

ADD_PLUGIN(EditorList
    SOURCES
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/editorlistwidget.cpp
        src/editorlistplugin.cpp
    HEADERS_MOC
        src/editorlistwidget.h
        src/editorlistplugin.h
    INCLUDE_DIRS
        src
    QT_MODULES
        Widgets
    QT_LIBRARIES
        QtCore
        QtGui
        QtWidgets
)
