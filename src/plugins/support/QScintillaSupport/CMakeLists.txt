PROJECT(QScintillaSupportPlugin)

# Add the plugin

ADD_PLUGIN(QScintillaSupport
    SOURCES
        ../../plugininfo.cpp

        src/qscintillawidget.cpp
        src/qscintillasupportplugin.cpp
    HEADERS_MOC
        src/qscintillawidget.h
        src/qscintillasupportplugin.h
    DEFINITIONS
        QSCINTILLA_DLL
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        QScintilla
    QT_MODULES
        Network
        Widgets
    QT_LIBRARIES
        QtCore
        QtGui
        QtNetwork
        QtWidgets
)
