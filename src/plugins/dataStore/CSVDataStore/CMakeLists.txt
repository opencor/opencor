PROJECT(CSVDataStorePlugin)

# Add the plugin

ADD_PLUGIN(CSVDataStore
    SOURCES
        ../../datastoreinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/csvdatastoreexporter.cpp
        src/csvdatastoreplugin.cpp
    HEADERS_MOC
        ../../datastoreinterface.h

        src/csvdatastoreplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        DataStore
)
