PROJECT(COMBINESupportPlugin)

# Add the plugin

ADD_PLUGIN(COMBINESupport
    SOURCES
        ../../filetypeinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp

        src/combinearchive.cpp
        src/combinearchiveissue.cpp
        src/combinefilemanager.cpp
        src/combinesupportplugin.cpp
    HEADERS_MOC
        src/combinearchive.h
        src/combinefilemanager.h
        src/combinesupportplugin.h
    PLUGINS
        StandardSupport
        ZIPSupport
    TESTS
        tests
)
