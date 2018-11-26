project(BioSignalMLDataStorePlugin)

# Add the plugin

add_plugin(BioSignalMLDataStore
    SOURCES
        ../../datastoreinterface.cpp
        ../../filetypeinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/biosignalmldatastoredata.cpp
        src/biosignalmldatastoredialog.cpp
        src/biosignalmldatastoreexporter.cpp
        src/biosignalmldatastoreimporter.cpp
        src/biosignalmldatastoreplugin.cpp
        src/biosignalmlinterface.cpp
    HEADERS_MOC
        ../../datastoreinterface.h

        src/biosignalmldatastoredialog.h
        src/biosignalmldatastoreexporter.h
        src/biosignalmldatastoreimporter.h
        src/biosignalmldatastoreplugin.h
    PLUGINS
        DataStore
        libBioSignalML
)
