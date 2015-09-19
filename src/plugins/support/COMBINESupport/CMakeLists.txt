PROJECT(COMBINESupportPlugin)

# Add the plugin

ADD_PLUGIN(COMBINESupport
    SOURCES
        ../../filetypeinterface.cpp
        ../../i18ninterface.cpp

        src/combinearchive.cpp
        src/combinesupportplugin.cpp
    HEADERS_MOC
        src/combinearchive.h
        src/combinesupportplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        Core
)
