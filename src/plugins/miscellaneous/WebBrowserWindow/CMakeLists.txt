project(WebBrowserWindowPlugin)

# Add the plugin

add_plugin(WebBrowserWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../windowinterface.cpp

        src/webbrowserwindowplugin.cpp
        src/webbrowserwindowwidget.cpp
        src/webbrowserwindowwindow.cpp
    UIS
        src/webbrowserwindowwindow.ui
    PLUGINS
        Core
        WebViewerWidget
    QT_MODULES
        PrintSupport
)
