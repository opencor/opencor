PROJECT(RawViewPlugin)

# Add the plugin

IF(APPLE)
    SET(QT_LIBRARIES_EXTRAS QtMacExtras)
ELSE()
    SET(QT_LIBRARIES_EXTRAS)
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
    QT_MODULES
        Widgets
        XmlPatterns
    QT_LIBRARIES
        ${QT_LIBRARIES_EXTRAS}
        QtCore
        QtGui
        QtNetwork
        QtWidgets
        QtXmlPatterns
)
