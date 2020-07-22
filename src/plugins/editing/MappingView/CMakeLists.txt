project(MappingViewPlugin)

# Add the plugin

add_plugin(MappingView
    SOURCES
        ../../filehandlinginterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../viewinterface.cpp

        src/mappingviewplugin.cpp
        src/mappingviewwidget.cpp
    UIS
        src/mappingviewwidget.ui
    PLUGINS
        Core
        CellMLSupport
)
