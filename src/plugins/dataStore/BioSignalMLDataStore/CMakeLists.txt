PROJECT(BioSignalMLDataStorePlugin)

# Add the plugin

ADD_PLUGIN(BioSignalMLDataStore
    SOURCES
        ../../datastoreinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/biosignalmldatastoreplugin.cpp
        src/biosignalmldatastoreexporter.cpp
        src/biosignalmldatastoresavedialog.cpp
        src/biosignalmldatastoreselectvars.cpp
    HEADERS_MOC
        src/biosignalmldatastoreplugin.h
        src/biosignalmldatastoresavedialog.h
        src/biosignalmldatastoreselectvars.h
    UIS
        src/biosignalmldatastoresavedialog.ui
        src/biosignalmldatastoreselectvars.ui
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        BioSignalMLAPI
    QT_MODULES
        Widgets
)
