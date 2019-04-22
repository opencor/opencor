project(WebViewerWidgetPlugin)

# Add the plugin

add_plugin(WebViewerWidget
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/webviewerwidget.cpp
        src/webviewerwidgetplugin.cpp
    PLUGINS
        Core
    QT_MODULES
        WebKitWidgets
)
