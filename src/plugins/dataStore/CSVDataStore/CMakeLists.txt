PROJECT(CSVDataStorePlugin)

# Add the plugin

ADD_PLUGIN(CSVDataStore
    SOURCES
        ../../plugininfo.cpp

        src/csvdatastoreexporter.cpp
        src/csvdatastoreplugin.cpp
    HEADERS_MOC
        src/csvdatastoreplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        CoreDataStore
    QT_MODULES
        Widgets
)
