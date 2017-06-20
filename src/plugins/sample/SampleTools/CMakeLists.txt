PROJECT(SampleToolsPlugin)

# Add the plugin

ADD_PLUGIN(SampleTools
    SOURCES
        ../../cliinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp

        src/sampletoolsplugin.cpp
    HEADERS_MOC
        src/sampletoolsplugin.h
    PLUGINS
        Core
        Sample
)
