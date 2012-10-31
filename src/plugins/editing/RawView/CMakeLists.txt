PROJECT(RawViewPlugin)

# Add the plugin

ADD_PLUGIN(RawView
    SOURCES
        ../../coreinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../interface.cpp
        ../../plugininfo.cpp

        src/rawviewplugin.cpp
        src/rawviewwidget.cpp
    HEADERS_MOC
        src/rawviewplugin.h
        src/rawviewwidget.h
    DEFINITIONS
        QSCINTILLA_DLL
    INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        Core
        QScintilla
        QScintillaSupport
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtNetwork
)
