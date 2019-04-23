project(CellMLAnnotationViewPlugin)

# Add the plugin

add_plugin(CellMLAnnotationView
    SOURCES
        ../../filehandlinginterface.cpp
        ../../i18ninterface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../pluginmanager.cpp
        ../../viewinterface.cpp

        src/cellmlannotationview.cpp
        src/cellmlannotationviewcellmllistwidget.cpp
        src/cellmlannotationvieweditingwidget.cpp
        src/cellmlannotationviewmetadatadetailswidget.cpp
        src/cellmlannotationviewmetadataeditdetailswidget.cpp
        src/cellmlannotationviewmetadatanormalviewdetailswidget.cpp
        src/cellmlannotationviewmetadatarawviewdetailswidget.cpp
        src/cellmlannotationviewmetadataviewdetailswidget.cpp
        src/cellmlannotationviewplugin.cpp
        src/cellmlannotationviewwidget.cpp
    PLUGINS
        CellMLSupport
        WebViewerWidget
)
