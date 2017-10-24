PROJECT(WebBrowserWindowPlugin)

# Add the plugin

ADD_PLUGIN(WebBrowserWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../pythoninterface.cpp
        ../../windowinterface.cpp

        src/webbrowserwindowplugin.cpp
        src/webbrowserwindowpythonwrapper.cpp
        src/webbrowserwindowwidget.cpp
        src/webbrowserwindowwindow.cpp
    HEADERS_MOC
        src/webbrowserwindowplugin.h
        src/webbrowserwindowpythonwrapper.h
        src/webbrowserwindowwidget.h
        src/webbrowserwindowwindow.h
    UIS
        src/webbrowserwindowwindow.ui
    PLUGINS
        Core
        PythonSupport
        WebViewerWidget
    QT_MODULES
        PrintSupport
)
