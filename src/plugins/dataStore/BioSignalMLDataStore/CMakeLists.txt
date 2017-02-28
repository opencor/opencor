PROJECT(BioSignalMLDataStorePlugin)

# Add the plugin

ADD_PLUGIN(BioSignalMLDataStore
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
        src/biosignalmldatastoreplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        BioSignalMLAPI
        DataStore
)
