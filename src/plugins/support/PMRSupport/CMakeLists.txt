PROJECT(PMRSupportPlugin)

# Add the plugin

ADD_PLUGIN(PMRSupport
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/pmrauthentication.cpp
        src/pmrexposure.cpp
        src/pmrsupportplugin.cpp
        src/pmrwebservice.cpp
        src/pmrwebservicemanager.cpp
        src/pmrwebserviceresponse.cpp
        src/pmrworkspace.cpp
        src/pmrworkspacefilenode.cpp
        src/pmrworkspacesmanager.cpp
    HEADERS_MOC
        src/pmrauthentication.h
        src/pmrexposure.h
        src/pmrsupportplugin.h
        src/pmrwebservice.h
        src/pmrwebservicemanager.h
        src/pmrwebserviceresponse.h
        src/pmrworkspace.h
        src/pmrworkspacefilenode.h
        src/pmrworkspacesmanager.h
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
