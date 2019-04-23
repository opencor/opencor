project(SampleWindowPlugin)

# Add the plugin

add_plugin(SampleWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../windowinterface.cpp

        src/samplewindowplugin.cpp
        src/samplewindowwindow.cpp
    UIS
        src/samplewindowwindow.ui
    PLUGINS
        Core
        Sample
)
