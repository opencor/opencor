project(BioSignalMLDataStorePlugin)

# Add the plugin

add_plugin(BioSignalMLDataStore
    SOURCES
        ../../datastoreinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/biosignalmldatastoredata.cpp
        src/biosignalmldatastoredialog.cpp
        src/biosignalmldatastoreexporter.cpp
        src/biosignalmldatastoreplugin.cpp
    HEADERS_MOC
        ../../datastoreinterface.h

        src/biosignalmldatastoredialog.h
        src/biosignalmldatastoreexporter.h
        src/biosignalmldatastoreplugin.h
    PLUGINS
        BioSignalML
        DataStore
)
