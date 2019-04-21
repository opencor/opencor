project(SampleViewPlugin)

# Add the plugin

add_plugin(SampleView
    SOURCES
        ../../filehandlinginterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../viewinterface.cpp

        src/sampleviewplugin.cpp
        src/sampleviewwidget.cpp
    UIS
        src/sampleviewwidget.ui
    PLUGINS
        Core
)
