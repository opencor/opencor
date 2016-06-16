PROJECT(SamplePlugin)

# Add the plugin

ADD_PLUGIN(Sample
    SOURCES
        ../../plugininfo.cpp

        src/sampleplugin.cpp
        src/sampleutilities.cpp
    HEADERS_MOC
        src/sampleplugin.h
    INCLUDE_DIRS
        src
    QT_MODULES
        Core
)
