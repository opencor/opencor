PROJECT(CoreEditingPlugin)

# Add the plugin

IF(APPLE)
    SET(QT_LIBRARIES_EXTRAS QtMacExtras)
ELSE()
    SET(QT_LIBRARIES_EXTRAS)
ENDIF()

ADD_PLUGIN(CoreEditing
    SOURCES
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/coreeditingplugin.cpp
    HEADERS_MOC
        src/coreeditingplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        QScintilla
        QScintillaSupport
    QT_MODULES
        Widgets
    QT_LIBRARIES
        ${QT_LIBRARIES_EXTRAS}
        QtCore
        QtGui
        QtNetwork
        QtWidgets
)
