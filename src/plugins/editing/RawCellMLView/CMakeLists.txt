project(RawCellMLViewPlugin)

# Add the plugin

add_plugin(RawCellMLView
    SOURCES
        ../../filehandlinginterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../viewinterface.cpp

        ../CellMLEditingView/src/cellmleditingviewinterface.cpp
        ../EditingView/src/editingviewinterface.cpp

        src/rawcellmlviewplugin.cpp
        src/rawcellmlviewwidget.cpp
    HEADERS_MOC
        src/rawcellmlviewplugin.h
        src/rawcellmlviewwidget.h
    PLUGINS
        CellMLEditingView
)
