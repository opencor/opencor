PROJECT(FileOrganiserWindowPlugin)

# Add the plugin

ADD_PLUGIN(FileOrganiserWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../windowinterface.cpp

        src/fileorganiserwindowplugin.cpp
        src/fileorganiserwindowwidget.cpp
        src/fileorganiserwindowwindow.cpp
    HEADERS_MOC
        src/fileorganiserwindowplugin.h
        src/fileorganiserwindowwidget.h
        src/fileorganiserwindowwindow.h
    UIS
        src/fileorganiserwindowwindow.ui
    PLUGINS
        Core
)
