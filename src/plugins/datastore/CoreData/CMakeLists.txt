PROJECT(CoreDataPlugin)

# Add the plugin

ADD_PLUGIN(CoreData
    SOURCES
        ../../plugininfo.cpp

        src/coredataplugin.cpp
        src/coredata.cpp
    HEADERS_MOC
        src/coredataplugin.h
        src/coredata.h
    INCLUDE_DIRS
        src
    QT_MODULES
        Core
    QT_LIBRARIES
        QtCore
)
