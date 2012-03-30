PROJECT(FileBrowserPlugin)

# Add the plugin

ADD_PLUGIN(FileBrowser
    SOURCES
        ../../coreinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../interface.cpp
        ../../plugininfo.cpp

        src/filebrowsermodel.cpp
        src/filebrowserplugin.cpp
        src/filebrowserwidget.cpp
        src/filebrowserwindow.cpp
    HEADERS_MOC
        src/filebrowsermodel.h
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
