PROJECT(RawViewPlugin)

# Add the plugin

ADD_PLUGIN(RawView
    SOURCES
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/rawviewplugin.cpp
        src/rawviewwidget.cpp
    HEADERS_MOC
        src/rawviewplugin.h
        src/rawviewwidget.h
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        QScintilla
        QScintillaSupport
    QT_MODULES
        Widgets
    QT_LIBRARIES
        QtCore
        QtGui
        QtNetwork
        QtWidgets
)
