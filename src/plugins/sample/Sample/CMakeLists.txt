PROJECT(SamplePlugin)

# Add the plugin

ADD_PLUGIN(Sample
    SOURCES
        ../../plugininfo.cpp

        src/sampleplugin.cpp
        src/sampleutilities.cpp
    HEADERS_MOC
        src/sampleplugin.h
    QT_MODULES
        Core
)
