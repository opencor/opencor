PROJECT(GraphPanelWidgetPlugin)

# Add the plugin

ADD_PLUGIN(GraphPanelWidget
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/graphpanelplotwidget.cpp
        src/graphpanelswidget.cpp
        src/graphpanelwidget.cpp
        src/graphpanelwidgetplugin.cpp
    HEADERS_MOC
        src/graphpanelplotwidget.h
        src/graphpanelswidget.h
        src/graphpanelwidget.h
        src/graphpanelwidgetplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        ${QWT_PLUGIN}
    PLUGIN_BINARIES
        ${QWT_PLUGIN_BINARY}
)
