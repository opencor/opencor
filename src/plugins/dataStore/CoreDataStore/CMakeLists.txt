PROJECT(CoreDataStorePlugin)

# Add the plugin

ADD_PLUGIN(CoreDataStore
    SOURCES
        ../../plugininfo.cpp

        src/coredatastoreplugin.cpp
        src/coredatastore.cpp
        src/datastorevariable.cpp
    HEADERS_MOC
        src/coredatastoreplugin.h
    INCLUDE_DIRS
        src
    QT_MODULES
        Core
    QT_LIBRARIES
        QtCore
)
