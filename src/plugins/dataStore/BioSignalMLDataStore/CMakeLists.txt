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
        ${biosignalml_INCLUDE_DIR}
        ${typedobject_INCLUDE_DIR}

    EXTERNAL_BINARIES_DIR
        ${biosignalml_LIBRARY_DIR}

    EXTERNAL_BINARIES
        libtypedobject.dylib
        libbiosignalml.dylib

    PLUGINS
        Core
        BioSignalMLAPI
    QT_MODULES
        Widgets
)
