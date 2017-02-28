PROJECT(CellMLAnnotationViewPlugin)

# Add the plugin

ADD_PLUGIN(CellMLAnnotationView
    SOURCES
        ../../filehandlinginterface.cpp
        ../../i18ninterface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../pluginmanager.cpp
        ../../viewinterface.cpp

        src/cellmlannotationviewcellmllistwidget.cpp
        src/cellmlannotationvieweditingwidget.cpp
        src/cellmlannotationviewmetadatadetailswidget.cpp
        src/cellmlannotationviewmetadataeditdetailswidget.cpp
        src/cellmlannotationviewmetadatanormalviewdetailswidget.cpp
        src/cellmlannotationviewmetadatarawviewdetailswidget.cpp
        src/cellmlannotationviewmetadataviewdetailswidget.cpp
        src/cellmlannotationviewplugin.cpp
        src/cellmlannotationviewwidget.cpp
    HEADERS_MOC
        ../../plugin.h
        ../../pluginmanager.h

        src/cellmlannotationviewcellmllistwidget.h
        src/cellmlannotationvieweditingwidget.h
        src/cellmlannotationviewmetadatadetailswidget.h
        src/cellmlannotationviewmetadataeditdetailswidget.h
        src/cellmlannotationviewmetadatanormalviewdetailswidget.h
        src/cellmlannotationviewmetadatarawviewdetailswidget.h
        src/cellmlannotationviewmetadataviewdetailswidget.h
        src/cellmlannotationviewplugin.h
        src/cellmlannotationviewwidget.h
    INCLUDE_DIRS
        src
    PLUGINS
        CellMLSupport
        WebViewerWidget
)
