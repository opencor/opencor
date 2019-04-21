project(SEDMLEditingViewPlugin)

# Add the plugin

add_plugin(SEDMLEditingView
    SOURCES
        ../../filehandlinginterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../pluginmanager.cpp

        src/sedmleditingviewinterface.cpp
        src/sedmleditingviewplugin.cpp
        src/sedmleditingviewwidget.cpp
    PLUGINS
        EditingView
        SEDMLSupport
)
