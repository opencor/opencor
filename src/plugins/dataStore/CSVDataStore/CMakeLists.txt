project(CSVDataStorePlugin)

# Add the plugin

add_plugin(CSVDataStore
    SOURCES
        ../../datastoreinterface.cpp
        ../../filetypeinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/csvdatastoreexporter.cpp
        src/csvdatastoreimporter.cpp
        src/csvdatastoreplugin.cpp
        src/csvinterface.cpp
    HEADERS_MOC
        ../../datastoreinterface.h

        src/csvdatastoreexporter.h
        src/csvdatastoreimporter.h
        src/csvdatastoreplugin.h
    PLUGINS
        DataStore
)
