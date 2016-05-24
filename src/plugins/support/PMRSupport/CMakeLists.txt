PROJECT(PMRSupportPlugin)

# Add the plugin

ADD_PLUGIN(PMRSupport
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/pmrauthentication.cpp
        src/pmrexposure.cpp
        src/pmrrepository.cpp
        src/pmrsupportplugin.cpp
    HEADERS_MOC
        src/pmrauthentication.h
        src/pmrrepository.h
        src/pmrsupportplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        ${LIBGIT2_PLUGIN}
        OAuth
        ${ZLIB_PLUGIN}
    PLUGIN_BINARIES
        ${LIBGIT2_PLUGIN_BINARY}
        ${ZLIB_PLUGIN_BINARY}
)
