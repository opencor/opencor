PROJECT(DataStorePlugin)

# Add the plugin

ADD_PLUGIN(DataStore
    SOURCES
        ../../datastoreinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/datastoreplugin.cpp
        src/datastorewindow.cpp
    HEADERS_MOC
        ../../datastoreinterface.h

        src/datastoreplugin.h
        src/datastorewindow.h
    UIS
        src/datastorewindow.ui
    INCLUDE_DIRS
        src
    PLUGINS
        Core
)
