project(SamplePlugin)

# Add the plugin

add_plugin(Sample
    SOURCES
        ../../plugininfo.cpp

        src/sampleplugin.cpp
        src/sampleutilities.cpp
    HEADERS_MOC
        src/sampleplugin.h
    QT_MODULES
        Core
)
