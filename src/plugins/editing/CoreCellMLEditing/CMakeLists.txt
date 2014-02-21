PROJECT(CoreCellMLEditingPlugin)

# Add the plugin

IF(APPLE)
    SET(QT_LIBRARIES_EXTRAS QtMacExtras)
ELSE()
    SET(QT_LIBRARIES_EXTRAS)
ENDIF()

ADD_PLUGIN(CoreCellMLEditing
    SOURCES
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../pluginmanager.cpp

        src/corecellmleditingplugin.cpp
        src/corecellmleditingwidget.cpp
    HEADERS_MOC
        ../../plugin.h
        ../../pluginmanager.h

        src/corecellmleditingplugin.h
        src/corecellmleditingwidget.h
    UIS
        src/corecellmleditingwidget.ui
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        QScintilla
        QScintillaSupport
        Qwt
        Viewer
    QT_MODULES
        Widgets
    QT_LIBRARIES
        ${QT_LIBRARIES_EXTRAS}
        QtConcurrent
        QtCore
        QtGui
        QtNetwork
        QtOpenGL
        QtPrintSupport
        QtSvg
        QtWidgets
        QtXml
)
