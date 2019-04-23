project(CellMLEditingViewPlugin)

# Add the plugin

add_plugin(CellMLEditingView
    SOURCES
        ../../filehandlinginterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../pluginmanager.cpp

        src/cellmleditingviewinterface.cpp
        src/cellmleditingviewplugin.cpp
        src/cellmleditingviewwidget.cpp
    PLUGINS
        CellMLSupport
        EditingView
        MathMLViewerWidget
)
