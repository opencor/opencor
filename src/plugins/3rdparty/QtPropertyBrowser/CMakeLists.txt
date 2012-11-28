PROJECT(QtPropertyBrowserPlugin)

# Add the plugin

ADD_PLUGIN(QtPropertyBrowser
    THIRD_PARTY
    SOURCES
        ../../plugininfo.cpp

        src/qtpropertybrowserplugin.cpp
    HEADERS_MOC
        src/qtpropertybrowserplugin.h
    INCLUDE_DIRS
        src
    QT_DEPENDENCIES
        QtCore
)
