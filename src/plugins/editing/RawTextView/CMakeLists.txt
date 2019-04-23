project(RawTextViewPlugin)

# Add the plugin

add_plugin(RawTextView
    SOURCES
        ../../filehandlinginterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../viewinterface.cpp

        ../EditingView/src/editingviewinterface.cpp

        src/rawtextviewplugin.cpp
        src/rawtextviewwidget.cpp
    PLUGINS
        EditingView
)
