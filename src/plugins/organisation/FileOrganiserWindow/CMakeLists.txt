PROJECT(FileOrganiserWindowPlugin)

# Add the plugin

ADD_PLUGIN(FileOrganiserWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/fileorganiserwindowmodel.cpp
        src/fileorganiserwindowplugin.cpp
        src/fileorganiserwindowwidget.cpp
        src/fileorganiserwindowwindow.cpp
    HEADERS_MOC
        src/fileorganiserwindowmodel.h
        src/fileorganiserwindowplugin.h
        src/fileorganiserwindowwidget.h
        src/fileorganiserwindowwindow.h
    UIS
        src/fileorganiserwindowwindow.ui
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
