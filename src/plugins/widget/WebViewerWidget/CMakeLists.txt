project(WebViewerWidgetPlugin)

# Add the plugin

add_plugin(WebViewerWidget
    SOURCES
        ../../plugininfo.cpp
        ../../plugininterface.cpp

        src/webviewerwidget.cpp
        src/webviewerwidgetplugin.cpp
    PLUGINS
        ToolBarWidget
    QT_MODULES
        PrintSupport
        WebKitWidgets
)
