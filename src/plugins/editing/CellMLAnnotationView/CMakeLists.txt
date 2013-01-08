PROJECT(CellMLAnnotationViewPlugin)

# Retrieve some CellML API and LLVM settings

RETRIEVE_CELLML_API_SETTINGS()
RETRIEVE_LLVM_SETTINGS()

# Add the plugin

ADD_PLUGIN(CellMLAnnotationView
    SOURCES
        ../../coreinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../interface.cpp
        ../../plugininfo.cpp

        src/cellmlannotationviewcellmlelementitem.cpp
        src/cellmlannotationviewcellmlelementitemdelegate.cpp
        src/cellmlannotationviewcellmllistwidget.cpp
        src/cellmlannotationviewmetadatadetailswidget.cpp
        src/cellmlannotationviewmetadataeditdetailswidget.cpp
        src/cellmlannotationviewmetadatanormalviewdetailswidget.cpp
        src/cellmlannotationviewmetadatarawviewdetailswidget.cpp
        src/cellmlannotationviewmetadataviewdetailswidget.cpp
        src/cellmlannotationviewplugin.cpp
        src/cellmlannotationviewwidget.cpp
    HEADERS_MOC
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
    OPENCOR_DEPENDENCIES
        CellMLAPI
        CellMLSupport
        Compiler
        Core
        CoreSolver
        ${LLVM_PLUGIN}
        QJson
        RICORDOSupport
    OPENCOR_BINARY_DEPENDENCIES
        ${LLVM_BINARY_PLUGIN}
    QT_MODULES
        WebKitWidgets
        Widgets
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtNetwork
        QtOpenGL
        QtPrintSupport
        QtQml
        QtQuick
        QtWebKit
        QtWebKitWidgets
        QtWidgets
    EXTERNAL_DEPENDENCIES
        ${CELLML_API_EXTERNAL_DEPENDENCIES}
)
