PROJECT(COMBINESupportPlugin)

# Add the plugin

ADD_PLUGIN(COMBINESupport
    SOURCES
        ../../filetypeinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/combinearchive.cpp
        src/combinearchiveissue.cpp
        src/combinefilemanager.cpp
        src/combinesupportplugin.cpp
    HEADERS_MOC
        src/combinearchive.h
        src/combinefilemanager.h
        src/combinesupportplugin.h
    INCLUDE_DIRS
        src
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
