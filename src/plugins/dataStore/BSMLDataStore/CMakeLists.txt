PROJECT(BSMLDataStorePlugin)

# Add the plugin

ADD_PLUGIN(BSMLDataStore
    SOURCES
        ../../plugininfo.cpp

        src/bsmldatastoreexporter.cpp
        src/bsmldatastoreplugin.cpp
    HEADERS_MOC
        src/bsmldatastoreplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        CoreDataStore
    QT_MODULES
        Widgets
    QT_LIBRARIES
        QtCore
        QtGui
        QtNetwork
        QtWidgets
        QtXml
        QtXmlPatterns
)
