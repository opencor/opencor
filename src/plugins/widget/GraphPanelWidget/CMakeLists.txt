project(GraphPanelWidgetPlugin)

# Add the plugin

add_plugin(GraphPanelWidget
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/graphpanelplotwidget.cpp
        src/graphpanelswidget.cpp
        src/graphpanelwidget.cpp
        src/graphpanelwidgetcustomaxesdialog.cpp
        src/graphpanelwidgetplugin.cpp
    UIS
        src/graphpanelwidgetcustomaxesdialog.ui
    PLUGINS
        Core
        Qwt
    QT_MODULES
        PrintSupport
)
