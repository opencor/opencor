PROJECT(CsvStorePlugin)

# Add the plugin

ADD_PLUGIN(CsvStore
    SOURCES
        ../../plugininfo.cpp

        src/csvstoreplugin.cpp
        src/csvstore.cpp
    HEADERS_MOC
        src/csvstoreplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        CoreData
    QT_MODULES
        Core
    QT_LIBRARIES
        QtCore
)
