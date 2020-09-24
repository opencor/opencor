project(ZincSupportPlugin)

# Add the plugin

add_plugin(ZincSupport
    SOURCES
        ../../filetypeinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp

        src/zincsupportplugin.cpp
        src/zincinterface.cpp
    PLUGINS
        ZincWidget
)
