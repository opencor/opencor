PROJECT(DataStorePlugin)

# Add the plugin

ADD_PLUGIN(DataStore
    SOURCES
        ../../datastoreinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../pythoninterface.cpp

        src/datastoredialog.cpp
        src/datastoreplugin.cpp
        src/datastorepythonwrapper.cpp
    HEADERS_MOC
        ../../datastoreinterface.h

        src/datastoredialog.h
        src/datastoreplugin.h
        src/datastorepythonwrapper.h
    UIS
        src/datastoredialog.ui
    PLUGINS
        Core
        PythonSupport
    DEPENDS_ON
        PythonPackagesPlugin
)
