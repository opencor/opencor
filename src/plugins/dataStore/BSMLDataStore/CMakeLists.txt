PROJECT(BSMLDataStorePlugin)

# Add the plugin

ADD_PLUGIN(BSMLDataStore
    SOURCES
        ../../plugininfo.cpp

        src/bsmldatastoreexporter.cpp
        src/bsmldatastoresavedialog.cpp
        src/bsmldatastoreplugin.cpp
    HEADERS_MOC
        src/bsmldatastoreplugin.h
        src/bsmldatastoresavedialog.h
    UIS
        src/bsmldatastoresavedialog.ui
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
        CoreDataStore
    QT_MODULES
        Widgets
)
