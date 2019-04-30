project(PMRWindowPlugin)

# Add the plugin

add_plugin(PMRWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../preferencesinterface.cpp
        ../../windowinterface.cpp

        src/pmrwindowplugin.cpp
        src/pmrwindowwidget.cpp
        src/pmrwindowwindow.cpp
    UIS
        src/pmrwindowwindow.ui
    PLUGINS
        PMRSupport
)
