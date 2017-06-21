PROJECT(FileBrowserWindowPlugin)

# Add the plugin

ADD_PLUGIN(FileBrowserWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../windowinterface.cpp

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
    PLUGINS
        Core
)
