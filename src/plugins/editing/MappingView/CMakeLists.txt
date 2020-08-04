project(MappingViewPlugin)

# Add the plugin

add_plugin(MappingView
    SOURCES
        ../../filehandlinginterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../viewinterface.cpp

        src/mappingvieweditingwidget.cpp
        src/mappingviewmeshreader.cpp
        src/mappingviewplugin.cpp
        src/mappingviewwidget.cpp
        src/mappingviewzincwidget.cpp
    PLUGINS
        Core
        CellMLSupport
        ZincWidget
        Qwt
)
