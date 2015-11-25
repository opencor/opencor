PROJECT(SampleToolsPlugin)

# Add the plugin

ADD_PLUGIN(SampleTools
    SOURCES
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/sampletoolsplugin.cpp
    HEADERS_MOC
        src/sampletoolsplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        Sample
)
