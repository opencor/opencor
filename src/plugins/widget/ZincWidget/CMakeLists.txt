project(ZincWidgetPlugin)

# Add the plugin

add_plugin(ZincWidget
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/zincwidget.cpp
        src/zincwidgetplugin.cpp
    HEADERS_MOC
        src/zincwidget.h
        src/zincwidgetplugin.h
    PLUGINS
        Core
        Zinc
)
