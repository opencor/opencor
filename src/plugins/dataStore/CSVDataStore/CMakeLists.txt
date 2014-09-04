PROJECT(CSVDataStorePlugin)

# Add the plugin

ADD_PLUGIN(CSVDataStore
    SOURCES
        ../../plugininfo.cpp

        src/csvdatastoreplugin.cpp
        src/csvdatastore.cpp
    HEADERS_MOC
        src/csvdatastoreplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        CoreDataStore
    QT_MODULES
        Widgets
    QT_LIBRARIES
        QtCore
        QtGui
        QtWidgets
)
