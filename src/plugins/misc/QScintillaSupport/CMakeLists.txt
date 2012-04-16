PROJECT(QScintillaSupportPlugin)

# Add the plugin

ADD_PLUGIN(QScintillaSupport
    SOURCES
        ../../plugininfo.cpp

        src/qscintilla.cpp
        src/qscintillasupportplugin.cpp
    HEADERS_MOC
        src/qscintillasupportplugin.h
    INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        Core
        QScintilla
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtNetwork
)
