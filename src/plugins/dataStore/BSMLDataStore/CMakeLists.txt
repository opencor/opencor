PROJECT(BSMLDataStorePlugin)

FIND_PACKAGE(biosignalml REQUIRED)
FIND_PACKAGE(typedobject REQUIRED)

find_library(typedobject_LIB typedobject PATHS ${typedobject_LIBRARY_DIR})
find_library(biosignalml_LIB biosignalml PATHS ${biosignalml_LIBRARY_DIR})

# Add the plugin

ADD_PLUGIN(BSMLDataStore
    SOURCES
        ../../plugininfo.cpp

        src/bsmldatastoreexporter.cpp
        src/bsmldatastoreplugin.cpp
    HEADERS_MOC
        src/bsmldatastoreplugin.h
    INCLUDE_DIRS
        src
        ${biosignalml_INCLUDE_DIR}
        ${typedobject_INCLUDE_DIR}

    EXTERNAL_BINARIES_DIR
        ${biosignalml_LIBRARY_DIR}

    EXTERNAL_BINARIES
        libtypedobject.dylib
        libbiosignalml.dylib
#        ${typedobject_LIB}
#        ${biosignalml_LIB}

    PLUGINS
        Core
        CoreDataStore
    QT_MODULES
        Widgets
)
