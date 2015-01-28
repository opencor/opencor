PROJECT(QScintillaSupportPlugin)

# Add the plugin

IF(APPLE)
    SET(QT_MAC_EXTRAS QtMacExtras)
ELSE()
    SET(QT_MAC_EXTRAS)
ENDIF()

ADD_PLUGIN(QScintillaSupport
    SOURCES
        ../../plugininfo.cpp

        src/qscintillasupportplugin.cpp
        src/qscintillawidget.cpp
    HEADERS_MOC
        src/qscintillasupportplugin.h
        src/qscintillawidget.h
    DEFINITIONS
        QSCINTILLA_DLL
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        ${QSCINTILLA_PLUGIN}
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
