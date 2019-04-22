project(ZincWindowPlugin)

# Add the plugin

add_plugin(ZincWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../windowinterface.cpp

        src/zincwindowplugin.cpp
        src/zincwindowwindow.cpp
    UIS
        src/zincwindowwindow.ui
    PLUGINS
        ZincWidget
)
