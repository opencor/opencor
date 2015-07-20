PROJECT(CoreDataStorePlugin)

# Add the plugin

ADD_PLUGIN(CoreDataStore
    SOURCES
        ../../plugininfo.cpp

        src/coredatastore.cpp
        src/coredatastoreplugin.cpp
        src/datastoreexporter.cpp
        src/datastorevariable.cpp
    HEADERS_MOC
        src/coredatastoreplugin.h
    INCLUDE_DIRS
        src
    QT_MODULES
        Core
)
