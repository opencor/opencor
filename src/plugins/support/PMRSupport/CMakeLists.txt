PROJECT(PMRSupportPlugin)

# Apply libgit2's and zlib's build settings

APPLY_LIBGIT2_BUILD_SETTINGS()
APPLY_ZLIB_BUILD_SETTINGS()

# Add the plugin

ADD_PLUGIN(PMRSupport
    SOURCES
        ../../plugininfo.cpp

        src/pmrsupportplugin.cpp
        src/pmrwebservice.cpp
    HEADERS_MOC
        src/pmrsupportplugin.h
        src/pmrwebservice.h
    INCLUDE_DIRS
        src
    DEFINITIONS
        ${LIBGIT2_DEFINITIONS}
        ${ZLIB_DEFINITIONS}
    PLUGINS
        Core
        ${LIBGIT2_PLUGIN}
        ${ZLIB_PLUGIN}
    PLUGIN_BINARIES
        ${LIBGIT2_PLUGIN_BINARY}
        ${ZLIB_PLUGIN_BINARY}
)
