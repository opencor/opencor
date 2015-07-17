PROJECT(APISupportPlugin)

# Add the plugin

ADD_PLUGIN(APISupport
    SOURCES
        ../../plugininfo.cpp

        src/apifile.cpp
        src/apifilemanager.cpp
        src/apisupportplugin.cpp
    HEADERS_MOC
        src/apifilemanager.h
        src/apisupportplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        Core
)
