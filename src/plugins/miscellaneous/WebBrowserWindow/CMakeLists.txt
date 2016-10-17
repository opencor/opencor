PROJECT(WebBrowserWindowPlugin)

# Add the plugin

ADD_PLUGIN(WebBrowserWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/webbrowserwindowplugin.cpp
        src/webbrowserwindowwindow.cpp
    HEADERS_MOC
        src/webbrowserwindowplugin.h
        src/webbrowserwindowwindow.h
    UIS
        src/webbrowserwindowwindow.ui
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        WebViewerWidget
    QT_MODULES
        PrintSupport
)
