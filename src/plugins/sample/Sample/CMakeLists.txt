project(SamplePlugin)

# Add the plugin

add_plugin(Sample
    SOURCES
        ../../plugininfo.cpp

        src/sampleplugin.cpp
        src/sampleutilities.cpp
    QT_MODULES
        Core
)
