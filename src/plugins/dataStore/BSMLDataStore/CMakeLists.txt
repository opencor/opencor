PROJECT(BSMLDataStorePlugin)

# Add the plugin

ADD_PLUGIN(BSMLDataStore
    SOURCES
        ../../datastoreinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/bsmldatastoreplugin.cpp
        src/bsmldatastoreexporter.cpp
        src/bsmldatastoresavedialog.cpp
        src/bsmldatastoreselectvars.cpp
    HEADERS_MOC
        src/bsmldatastoreplugin.h
        src/bsmldatastoresavedialog.h
        src/bsmldatastoreselectvars.h
    UIS
        src/bsmldatastoresavedialog.ui
        src/bsmldatastoreselectvars.ui
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
