PROJECT(QtPropertyBrowserSupportPlugin)

# Add the plugin

ADD_PLUGIN(QtPropertyBrowserSupport
    SOURCES
        ../../plugininfo.cpp

        src/qtpropertybrowserwidget.cpp
        src/qtpropertybrowsersupportplugin.cpp
    HEADERS_MOC
        src/qtpropertybrowserwidget.h
        src/qtpropertybrowsersupportplugin.h
    INCLUDE_DIRS
        src
    DEFINITIONS
        QT_QTPROPERTYBROWSER_IMPORT
    OPENCOR_DEPENDENCIES
        Core
        QtPropertyBrowser
    QT_DEPENDENCIES
        QtCore
        QtGui
)
