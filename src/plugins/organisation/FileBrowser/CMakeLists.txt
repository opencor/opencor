PROJECT(FileBrowserPlugin)

# Add the plugin

ADD_PLUGIN(FileBrowser
    SOURCES
        ../../coreinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/filebrowserplugin.cpp
        src/filebrowserwidget.cpp
        src/filebrowserwindow.cpp
    HEADERS
        ../../coreinterface.h
        ../../guiinterface.h
        ../../i18ninterface.h
        ../../plugininfo.h
    HEADERS_MOC
        src/filebrowserplugin.h
        src/filebrowserwidget.h
        src/filebrowserwindow.h
    UIS
        src/filebrowserwindow.ui
    INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        Core
    QT_DEPENDENCIES
        QtCore
        QtGui
)
