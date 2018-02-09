project(COMBINESupportPlugin)

# Add the plugin

add_plugin(COMBINESupport
    SOURCES
        ../../filetypeinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp

        src/combinearchive.cpp
        src/combinearchiveissue.cpp
        src/combinefilemanager.cpp
        src/combineinterface.cpp
        src/combinesupportplugin.cpp
    HEADERS_MOC
        src/combinearchive.h
        src/combinefilemanager.h
        src/combinesupportplugin.h
    PLUGINS
        SEDMLSupport
        ZIPSupport
    TESTS
        tests
)
