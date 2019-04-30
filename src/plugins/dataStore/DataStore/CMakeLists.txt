project(DataStorePlugin)

# Add the plugin

add_plugin(DataStore
    SOURCES
        ../../datastoreinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../pythoninterface.cpp

        src/datastoredialog.cpp
        src/datastoreplugin.cpp
        src/datastorepythonwrapper.cpp
    UIS
        src/datastoredialog.ui
    PLUGINS
        Core
        PythonQtSupport
    DEPENDS_ON
        PythonPackagesPlugin
)
