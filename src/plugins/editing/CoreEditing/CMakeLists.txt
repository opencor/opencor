PROJECT(CoreEditingPlugin)

# Add the plugin

IF(APPLE)
    SET(QT_MAC_EXTRAS QtMacExtras)
ELSE()
    SET(QT_MAC_EXTRAS)
ENDIF()

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
        ${QT_MAC_EXTRAS}
        QtNetwork
        QtWidgets
        QtXml
        QtXmlPatterns
)
