PROJECT(WebBrowserWindowPlugin)

# Add the plugin

ADD_PLUGIN(WebBrowserWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../windowinterface.cpp

        src/webbrowserwindowplugin.cpp
        src/webbrowserwindowwidget.cpp
        src/webbrowserwindowwindow.cpp
    HEADERS_MOC
        src/webbrowserwindowplugin.h
        src/webbrowserwindowwidget.h
        src/webbrowserwindowwindow.h
    UIS
        src/webbrowserwindowwindow.ui
    PLUGINS
        Core
        WebViewerWidget
    QT_MODULES
        PrintSupport
)
