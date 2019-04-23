project(RawSEDMLViewPlugin)

# Add the plugin

add_plugin(RawSEDMLView
    SOURCES
        ../../filehandlinginterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../viewinterface.cpp

        ../EditingView/src/editingviewinterface.cpp
        ../SEDMLEditingView/src/sedmleditingviewinterface.cpp

        src/rawsedmlviewplugin.cpp
        src/rawsedmlviewwidget.cpp
    PLUGINS
        SEDMLEditingView
)
