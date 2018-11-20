project(CSVDataStorePlugin)

# Add the plugin

add_plugin(CSVDataStore
    SOURCES
        ../../datastoreinterface.cpp
        ../../filetypeinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/csvdatastoreexporter.cpp
        src/csvdatastoreplugin.cpp
    HEADERS_MOC
        ../../datastoreinterface.h

        src/csvdatastoreexporter.h
        src/csvdatastoreplugin.h
    PLUGINS
        DataStore
)
