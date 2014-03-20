PROJECT(EditorPlugin)

# Add the plugin

IF(APPLE)
    SET(QT_LIBRARIES_EXTRAS QtMacExtras)
ELSE()
    SET(QT_LIBRARIES_EXTRAS)
ENDIF()

ADD_PLUGIN(Editor
    SOURCES
        ../../plugininfo.cpp

        src/editorwidget.cpp
        src/editorplugin.cpp
    HEADERS_MOC
        src/editorwidget.h
        src/editorplugin.h
    UIS
        src/editorwidget.ui
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
        ${QT_LIBRARIES_EXTRAS}
        QtCore
        QtGui
        QtNetwork
        QtWidgets
        QtXmlPatterns
)
