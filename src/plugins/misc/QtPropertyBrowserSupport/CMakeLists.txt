PROJECT(QtPropertyBrowserSupportPlugin)

# Add the plugin

ADD_PLUGIN(QtPropertyBrowserSupport
    SOURCES
        ../../i18ninterface.cpp
        ../../interface.cpp
        ../../plugininfo.cpp

        src/qtpropertybrowserdoubleeditorfactory.cpp
        src/qtpropertybrowserdoubleeditorwidget.cpp
        src/qtpropertybrowserdoublepropertymanager.cpp
        src/qtpropertybrowserwidget.cpp
        src/qtpropertybrowsersupportplugin.cpp
    HEADERS_MOC
        src/qtpropertybrowserdoubleeditorfactory.h
        src/qtpropertybrowserdoubleeditorwidget.h
        src/qtpropertybrowserdoublepropertymanager.h
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
