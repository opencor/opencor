PROJECT(CoreCellMLEditingPlugin)

# Retrieve some CellML API and LLVM settings

RETRIEVE_CELLML_API_SETTINGS()
RETRIEVE_LLVM_SETTINGS()

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
    DEFINITIONS
        ${LLVM_DEFINITIONS}
    PLUGINS
        Core
        ${LLVM_PLUGIN}
        QScintilla
        QScintillaSupport
        Qwt
        Viewer
    PLUGIN_BINARIES
        ${LLVM_BINARY_PLUGIN}
    QT_MODULES
        Widgets
        Xml
        XmlPatterns
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
    EXTERNAL_BINARIES
        ${CELLML_API_EXTERNAL_BINARIES}
)
