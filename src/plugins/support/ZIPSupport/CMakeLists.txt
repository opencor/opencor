project(ZIPSupportPlugin)

# Add the plugin

add_plugin(ZIPSupport
    SOURCES
        ../../plugininfo.cpp

        src/qzip.cpp
        src/zipsupportplugin.cpp
    PLUGINS
        Core
        zlib
    TESTS
        tests
)
