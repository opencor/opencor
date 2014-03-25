PROJECT(EditorPlugin)

# Add the plugin

IF(APPLE)
    SET(QT_LIBRARIES_EXTRAS QtMacExtras)
ELSE()
    SET(QT_LIBRARIES_EXTRAS)
ENDIF()

ADD_PLUGIN(Editor
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/editorfindreplacewidget.cpp
        src/editorwidget.cpp
        src/editorplugin.cpp
    HEADERS_MOC
        src/editorfindreplacewidget.h
        src/editorwidget.h
        src/editorplugin.h
    UIS
        src/editorfindreplacewidget.ui
        src/editorwidget.ui
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        QScintilla
        QScintillaSupport
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
