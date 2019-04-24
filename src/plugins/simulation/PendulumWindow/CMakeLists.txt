PROJECT(PendulumWindowPlugin)

# Add the plugin

ADD_PLUGIN(PendulumWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../windowinterface.cpp

        src/pendulumwindowplugin.cpp
        src/pendulumwindowwindow.cpp
    UIS
        src/pendulumwindowwindow.ui
    PLUGINS
        ZincWidget
)
