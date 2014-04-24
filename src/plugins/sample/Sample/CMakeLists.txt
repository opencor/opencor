PROJECT(SamplePlugin)

# Add the plugin

ADD_PLUGIN(Sample
    SOURCES
        ../../plugininfo.cpp

        src/sampleplugin.cpp
        src/sampleutils.cpp
    INCLUDE_DIRS
        src
    QT_MODULES
        Core
    QT_LIBRARIES
        QtCore
)
