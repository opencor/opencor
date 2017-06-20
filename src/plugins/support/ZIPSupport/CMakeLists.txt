PROJECT(ZIPSupportPlugin)

# Add the plugin

ADD_PLUGIN(ZIPSupport
    SOURCES
        ../../plugininfo.cpp

        src/qzip.cpp
        src/zipsupportplugin.cpp
    HEADERS_MOC
        src/zipsupportplugin.h
    PLUGINS
        Core
        zlib
    TESTS
        tests
)
