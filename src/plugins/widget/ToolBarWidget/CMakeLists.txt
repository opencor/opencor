project(ToolBarWidgetPlugin)

# Add the plugin

add_plugin(ToolBarWidget
    SOURCES
        ../../plugininfo.cpp

        src/toolbarwidget.cpp
        src/toolbarwidgetdropdownlistwidgetaction.cpp
        src/toolbarwidgetlabelwidgetaction.cpp
        src/toolbarwidgetlineeditwidgetaction.cpp
        src/toolbarwidgetplugin.cpp
        src/toolbarwidgetwheelwidgetaction.cpp
    PLUGINS
        Core
        Qwt
)
