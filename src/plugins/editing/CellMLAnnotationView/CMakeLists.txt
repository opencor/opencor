PROJECT(CellMLAnnotationViewPlugin)

# Retrieve some CellML API and LLVM settings

RETRIEVE_CELLML_API_SETTINGS()
RETRIEVE_LLVM_SETTINGS()

# Add the plugin

ADD_PLUGIN(CellMLAnnotationView
    SOURCES
        ../../i18ninterface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../pluginmanager.cpp
        ../../viewinterface.cpp

        src/cellmlannotationviewcellmllistwidget.cpp
        src/cellmlannotationvieweditingwidget.cpp
        src/cellmlannotationviewmetadatadetailswidget.cpp
        src/cellmlannotationviewmetadataeditdetailswidget.cpp
        src/cellmlannotationviewmetadatanormalviewdetailswidget.cpp
        src/cellmlannotationviewmetadatarawviewdetailswidget.cpp
        src/cellmlannotationviewmetadataviewdetailswidget.cpp
        src/cellmlannotationviewmetadatawebviewwidget.cpp
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
        src/cellmlannotationviewmetadatawebviewwidget.h
        src/cellmlannotationviewplugin.h
        src/cellmlannotationviewwidget.h
    UIS
        src/cellmlannotationviewcellmllistwidget.ui
        src/cellmlannotationvieweditingwidget.ui
        src/cellmlannotationviewmetadatadetailswidget.ui
        src/cellmlannotationviewmetadataeditdetailswidget.ui
        src/cellmlannotationviewmetadatanormalviewdetailswidget.ui
        src/cellmlannotationviewwidget.ui
    INCLUDE_DIRS
        src
    DEFINITIONS
        ${LLVM_DEFINITIONS}
    PLUGINS
        CellMLAPI
        CellMLSupport
        Compiler
        Core
        CoreSolver
        ${LLVM_PLUGIN}
    PLUGIN_BINARIES
        ${LLVM_PLUGIN_BINARY}
    QT_MODULES
        WebKitWidgets
    QT_LIBRARIES
        QtCore
        QtGui
        QtNetwork
        QtWebKit
        QtWebKitWidgets
        QtWidgets
        QtXml
        QtXmlPatterns
    EXTERNAL_BINARIES
        ${CELLML_API_EXTERNAL_BINARIES}
)
