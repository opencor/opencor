PROJECT(SampleToolsPlugin)

# Add the plugin

ADD_PLUGIN(SampleTools
    SOURCES
        ../../plugininfo.cpp

        src/sampletoolsplugin.cpp
    INCLUDE_DIRS
        src
    PLUGINS
        Sample
    QT_MODULES
        Core
    QT_LIBRARIES
        QtCore
)
