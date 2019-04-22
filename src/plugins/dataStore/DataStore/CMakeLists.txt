project(DataStorePlugin)

# Add the plugin

add_plugin(DataStore
    SOURCES
        ../../datastoreinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/datastoredialog.cpp
        src/datastoreplugin.cpp
    UIS
        src/datastoredialog.ui
    PLUGINS
        Core
)
