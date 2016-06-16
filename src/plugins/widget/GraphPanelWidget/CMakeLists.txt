PROJECT(GraphPanelWidgetPlugin)

# Add the plugin

ADD_PLUGIN(GraphPanelWidget
    SOURCES
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/graphpanelwidgetplugin.cpp
    HEADERS_MOC
        src/graphpanelwidgetplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        Core
)
