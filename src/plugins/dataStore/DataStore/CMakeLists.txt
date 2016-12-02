PROJECT(DataStorePlugin)

# Add the plugin

ADD_PLUGIN(DataStore
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/datastoredialog.cpp
        src/datastoreplugin.cpp
    HEADERS_MOC
        src/datastoredialog.h
        src/datastoreplugin.h
    UIS
        src/datastoredialog.ui
    INCLUDE_DIRS
        src
    PLUGINS
        Core
)
