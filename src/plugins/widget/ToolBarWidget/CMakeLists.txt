project(ToolBarWidgetPlugin)

# Add the plugin

add_plugin(ToolBarWidget
    SOURCES
        ../../plugininfo.cpp

        src/toolbarwidget.cpp
        src/toolbarwidgetplugin.cpp
    PLUGINS
        Core
        Qwt
)
