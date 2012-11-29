PROJECT(QtPropertyBrowserPlugin)

# Add the plugin

ADD_PLUGIN(QtPropertyBrowser
    THIRD_PARTY
    SOURCES
        ../../plugininfo.cpp

        src/qtbuttonpropertybrowser.cpp
        src/qteditorfactory.cpp
        src/qtgroupboxpropertybrowser.cpp
        src/qtpropertybrowser.cpp
        src/qtpropertybrowserplugin.cpp
        src/qtpropertybrowserutils.cpp
        src/qtpropertymanager.cpp
        src/qttreepropertybrowser.cpp
        src/qtvariantproperty.cpp
    SOURCES_TO_MOC
        src/qteditorfactory.cpp
        src/qtpropertymanager.cpp
        src/qttreepropertybrowser.cpp
    HEADERS_MOC
        src/qtpropertybrowserplugin.h
        src/qtpropertybrowserutils_p.h
    HEADERS_MOC_TO_CPP
        src/qtbuttonpropertybrowser.h
        src/qteditorfactory.h
        src/qtgroupboxpropertybrowser.h
        src/qtpropertybrowser.h
        src/qtpropertymanager.h
        src/qttreepropertybrowser.h
        src/qtvariantproperty.h
    RESOURCES
        src/qtpropertybrowser.qrc
    INCLUDE_DIRS
        src
    DEFINITIONS
        QT_QTPROPERTYBROWSER_EXPORT
    QT_DEPENDENCIES
        QtCore
        QtGui
)
