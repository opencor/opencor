PROJECT(FileBrowserWindowPlugin)

# Add the plugin

ADD_PLUGIN(FileBrowserWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/filebrowserwindowmodel.cpp
        src/filebrowserwindowplugin.cpp
        src/filebrowserwindowwidget.cpp
        src/filebrowserwindowwindow.cpp
    HEADERS_MOC
        src/filebrowserwindowmodel.h
        src/filebrowserwindowplugin.h
        src/filebrowserwindowwidget.h
        src/filebrowserwindowwindow.h
    UIS
        src/filebrowserwindowwindow.ui
    INCLUDE_DIRS
        src
    PLUGINS
        Core
    QT_LIBRARIES
        QtCore
        QtGui
        QtNetwork
        QtWidgets
        QtXml
        QtXmlPatterns
)
