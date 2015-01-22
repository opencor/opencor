PROJECT(CoreEditingPlugin)

# Add the plugin

ADD_PLUGIN(CoreEditing
    SOURCES
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../pluginmanager.cpp

        src/coreeditingplugin.cpp
    HEADERS_MOC
        ../../plugin.h
        ../../pluginmanager.h

        src/coreeditingplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        Editor
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
