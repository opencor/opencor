PROJECT(DataStorePlugin)

# Add the plugin

ADD_PLUGIN(DataStore
    SOURCES
        ../../datastoreinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/datastoredialog.cpp
        src/datastoreplugin.cpp
    HEADERS_MOC
        ../../datastoreinterface.h

        src/datastoredialog.h
        src/datastoreplugin.h
    UIS
        src/datastoredialog.ui
    PLUGINS
        Core
)
