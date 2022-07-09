project(BondGraphEditorWindowPlugin)

# Add the plugin

add_plugin(BondGraphEditorWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../windowinterface.cpp

        src/bondgrapheditorwindowplugin.cpp
        src/bondgrapheditorwindow.cpp
    PLUGINS
        BondGraphUIWidgets
        BondGraphAPI
        Core
)
