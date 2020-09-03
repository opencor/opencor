project(CellMLZincMappingViewPlugin)

# Add the plugin

add_plugin(CellMLZincMappingView
    SOURCES
        ../../filehandlinginterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../viewinterface.cpp

        src/cellmlzincmappingvieweditingwidget.cpp
        src/cellmlzincmappingviewplugin.cpp
        src/cellmlzincmappingviewwidget.cpp
        src/cellmlzincmappingviewzincwidget.cpp
    PLUGINS
        Core
        CellMLSupport
        Qwt
        ZincWidget
)
