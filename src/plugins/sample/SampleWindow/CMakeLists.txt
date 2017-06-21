PROJECT(SampleWindowPlugin)

# Add the plugin

ADD_PLUGIN(SampleWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../windowinterface.cpp

        src/samplewindowplugin.cpp
        src/samplewindowwindow.cpp
    HEADERS_MOC
        src/samplewindowplugin.h
        src/samplewindowwindow.h
    UIS
        src/samplewindowwindow.ui
    PLUGINS
        Core
        Sample
)
