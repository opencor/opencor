PROJECT(CoreDatastorePlugin)

# Add the plugin

ADD_PLUGIN(CoreDatastore
    SOURCES
        ../../plugininfo.cpp

        src/coredatastoreplugin.cpp
        src/coredatastore.cpp
    HEADERS_MOC
        src/coredatastoreplugin.h
        src/coredatastore.h
    INCLUDE_DIRS
        src
    QT_MODULES
        Core
    QT_LIBRARIES
        QtCore
)
