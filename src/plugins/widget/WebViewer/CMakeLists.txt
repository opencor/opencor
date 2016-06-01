PROJECT(WebViewerPlugin)

# Add the plugin

ADD_PLUGIN(WebViewer
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/webviewerplugin.cpp
        src/webviewerwidget.cpp
    HEADERS_MOC
        src/webviewerplugin.h
        src/webviewerwidget.h
    INCLUDE_DIRS
        src
    QT_MODULES
        WebKitWidgets
)
