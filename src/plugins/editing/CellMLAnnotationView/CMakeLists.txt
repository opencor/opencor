PROJECT(CellMLAnnotationViewPlugin)

# Retrieve some CellML API and LLVM settings

RETRIEVE_CELLML_API_SETTINGS()
RETRIEVE_LLVM_SETTINGS()

# Add the plugin

ADD_PLUGIN(CellMLAnnotationView
    SOURCES
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../pluginmanager.cpp

        src/cellmlannotationviewcellmllistwidget.cpp
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
        src/cellmlannotationviewmetadatadetailswidget.h
        src/cellmlannotationviewmetadataeditdetailswidget.h
        src/cellmlannotationviewmetadatanormalviewdetailswidget.h
        src/cellmlannotationviewmetadatarawviewdetailswidget.h
        src/cellmlannotationviewmetadataviewdetailswidget.h
        src/cellmlannotationviewplugin.h
        src/cellmlannotationviewwidget.h
    UIS
        src/cellmlannotationviewcellmllistwidget.ui
        src/cellmlannotationviewmetadatadetailswidget.ui
        src/cellmlannotationviewmetadataeditdetailswidget.ui
        src/cellmlannotationviewmetadatanormalviewdetailswidget.ui
        src/cellmlannotationviewwidget.ui
    DEFINITIONS
        ${LLVM_DEFINITIONS}
    INCLUDE_DIRS
        src
    PLUGINS
        CellMLAPI
        CellMLSupport
        Compiler
        Core
        CoreSolver
        ${LLVM_PLUGIN}
    PLUGIN_BINARIES
        ${LLVM_BINARY_PLUGIN}
    QT_MODULES
        WebKitWidgets
    QT_LIBRARIES
        QtCore
        QtGui
        QtMultimedia
        QtMultimediaWidgets
        QtNetwork
        QtOpenGL
        QtPositioning
        QtPrintSupport
        QtQml
        QtQuick
        QtSensors
        QtWebKit
        QtWebKitWidgets
        QtWidgets
    EXTERNAL_BINARIES
        ${CELLML_API_EXTERNAL_BINARIES}
)
