project(WebBrowserWindowPlugin)

# Add the plugin

add_plugin(WebBrowserWindow
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
        PythonQtSupport
        WebViewerWidget
    QT_MODULES
        PrintSupport
)
