PROJECT(WebViewerWidgetPlugin)

# Add the plugin

ADD_PLUGIN(WebViewerWidget
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/webviewerwidget.cpp
        src/webviewerwidgetplugin.cpp
    HEADERS_MOC
        src/webviewerwidget.h
        src/webviewerwidgetplugin.h
    INCLUDE_DIRS
        src
    QT_MODULES
        WebKitWidgets
)
