PROJECT(ZIPSupportPlugin)

# Add the plugin

ADD_PLUGIN(ZIPSupport
    SOURCES
        ../../plugininfo.cpp

        src/qzip.cpp
        src/zipsupportplugin.cpp
    HEADERS_MOC
        src/zipsupportplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        ${ZLIB_PLUGIN}
    PLUGIN_BINARIES
        ${ZLIB_PLUGIN_BINARY}
    TESTS
        tests
)
