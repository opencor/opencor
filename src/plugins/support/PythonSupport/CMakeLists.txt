PROJECT(PythonSupportPlugin)

# Add the plugin

ADD_PLUGIN(PythonSupport
    SOURCES
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp

        src/pythonsupportplugin.cpp
    HEADERS_MOC
        ../../plugin.h
        ../../pluginmanager.h

        src/pythonsupportplugin.h
    PLUGINS
        Core
        Python
)
