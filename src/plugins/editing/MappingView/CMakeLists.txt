project(MappingViewPlugin)

# Add the plugin

add_plugin(MappingView
    SOURCES
        ../../filehandlinginterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../viewinterface.cpp

        ../../windowinterface.cpp

        src/mappingvieweditingwidget.cpp
        src/mappingviewplugin.cpp
        src/mappingviewwidget.cpp
        src/mappingviewzincwidget.cpp
        src/meshreader.cpp
    PLUGINS
        Core
        CellMLSupport
        ZincWidget
)
