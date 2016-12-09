PROJECT(PMRSupportPlugin)

# Add the plugin

ADD_PLUGIN(PMRSupport
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/pmrexposure.cpp
        src/pmrsupportplugin.cpp
        src/pmrwebservice.cpp
    HEADERS_MOC
        src/pmrsupportplugin.h
        src/pmrwebservice.h
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        ${LIBGIT2_PLUGIN}
        ${ZLIB_PLUGIN}
    PLUGIN_BINARIES
        ${LIBGIT2_PLUGIN_BINARY}
        ${ZLIB_PLUGIN_BINARY}
)
