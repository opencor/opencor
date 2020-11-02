project(FileBrowserWindowPlugin)

# Add the plugin

add_plugin(FileBrowserWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../windowinterface.cpp

        src/filebrowserwindowmodel.cpp
        src/filebrowserwindowplugin.cpp
        src/filebrowserwindowwidget.cpp
        src/filebrowserwindowwindow.cpp
    UIS
        src/filebrowserwindowwindow.ui
    PLUGINS
        ToolBarWidget
)
