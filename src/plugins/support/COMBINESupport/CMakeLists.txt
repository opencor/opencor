PROJECT(COMBINESupportPlugin)

# Apply zlib's build settings

APPLY_ZLIB_BUILD_SETTINGS()

# Add the plugin

ADD_PLUGIN(COMBINESupport
    SOURCES
        ../../filetypeinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/combinearchive.cpp
        src/combinesupportplugin.cpp
    HEADERS_MOC
        src/combinearchive.h
        src/combinesupportplugin.h
    INCLUDE_DIRS
        src
    DEFINITIONS
        ${ZLIB_DEFINITIONS}
    PLUGINS
        Core
        StandardSupport
        ZIPSupport
        ${ZLIB_PLUGIN}
    PLUGIN_BINARIES
        ${ZLIB_PLUGIN_BINARY}
    TESTS
        tests
)
