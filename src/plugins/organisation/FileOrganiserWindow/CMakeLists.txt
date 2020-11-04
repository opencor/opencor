project(FileOrganiserWindowPlugin)

# Add the plugin

add_plugin(FileOrganiserWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../windowinterface.cpp

        src/fileorganiserwindowplugin.cpp
        src/fileorganiserwindowwidget.cpp
        src/fileorganiserwindowwindow.cpp
    UIS
        src/fileorganiserwindowwindow.ui
    PLUGINS
        ToolBarWidget
)
