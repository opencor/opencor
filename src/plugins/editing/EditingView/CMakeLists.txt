project(EditingViewPlugin)

# Add the plugin

add_plugin(EditingView
    SOURCES
        ../../filehandlinginterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../pluginmanager.cpp

        src/editingviewinterface.cpp
        src/editingviewplugin.cpp
    PLUGINS
        EditorWidget
)
