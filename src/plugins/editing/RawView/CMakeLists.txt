PROJECT(RawViewPlugin)

# Add the plugin

IF(APPLE)
    SET(QT_MAC_EXTRAS QtMacExtras)
ELSE()
    SET(QT_MAC_EXTRAS)
ENDIF()

ADD_PLUGIN(RawView
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../viewinterface.cpp

        src/rawviewplugin.cpp
        src/rawviewwidget.cpp
    HEADERS_MOC
        src/rawviewplugin.h
        src/rawviewwidget.h
    UIS
        src/rawviewwidget.ui
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
