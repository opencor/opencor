PROJECT(SampleViewPlugin)

# Add the plugin

ADD_PLUGIN(SampleView
    SOURCES
        ../../filehandlinginterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../viewinterface.cpp

        src/sampleviewplugin.cpp
        src/sampleviewwidget.cpp
    HEADERS_MOC
        src/sampleviewplugin.h
        src/sampleviewwidget.h
    UIS
        src/sampleviewwidget.ui
    PLUGINS
        Core
)
