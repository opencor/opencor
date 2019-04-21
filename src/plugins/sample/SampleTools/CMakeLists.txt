project(SampleToolsPlugin)

# Add the plugin

add_plugin(SampleTools
    SOURCES
        ../../cliinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp

        src/sampletoolsplugin.cpp
    PLUGINS
        Core
        Sample
)
