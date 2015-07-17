PROJECT(StandardSupportPlugin)

# Add the plugin

ADD_PLUGIN(StandardSupport
    SOURCES
        ../../plugininfo.cpp

        src/standardfile.cpp
        src/standardfilemanager.cpp
        src/standardsupportplugin.cpp
    HEADERS_MOC
        src/standardfilemanager.h
        src/standardsupportplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        Core
)
