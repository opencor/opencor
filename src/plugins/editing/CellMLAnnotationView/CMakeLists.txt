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
        src/cellmlannotationviewplugin.cpp
        src/cellmlannotationviewcellmldetailswidget.cpp
        src/cellmlannotationviewcellmlelementdetailswidget.cpp
        src/cellmlannotationviewdetailswidget.cpp
        src/cellmlannotationviewlistswidget.cpp
        src/cellmlannotationviewmetadatadetailswidget.cpp
        src/cellmlannotationviewmetadatabiomodelsdotnetviewdetailswidget.cpp
        src/cellmlannotationviewmetadatarawviewdetailswidget.cpp
        src/cellmlannotationviewmetadataviewdetailswidget.cpp
        src/cellmlannotationviewwidget.cpp
    HEADERS_MOC
        src/cellmlannotationviewplugin.h
        src/cellmlannotationviewcellmldetailswidget.h
        src/cellmlannotationviewcellmlelementdetailswidget.h
        src/cellmlannotationviewdetailswidget.h
        src/cellmlannotationviewlistswidget.h
        src/cellmlannotationviewmetadatadetailswidget.h
        src/cellmlannotationviewmetadatabiomodelsdotnetviewdetailswidget.h
        src/cellmlannotationviewmetadatarawviewdetailswidget.h
        src/cellmlannotationviewmetadataviewdetailswidget.h
        src/cellmlannotationviewwidget.h
    UIS
        src/cellmlannotationviewcellmldetailswidget.ui
        src/cellmlannotationviewcellmlelementdetailswidget.ui
        src/cellmlannotationviewdetailswidget.ui
        src/cellmlannotationviewlistswidget.ui
        src/cellmlannotationviewmetadatadetailswidget.ui
        src/cellmlannotationviewmetadatabiomodelsdotnetviewdetailswidget.ui
        src/cellmlannotationviewmetadatarawviewdetailswidget.ui
        src/cellmlannotationviewmetadataviewdetailswidget.ui
        src/cellmlannotationviewwidget.ui
    DEFINITIONS
        ${LLVM_DEFINITIONS}
    INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        CellMLAPI
        CellMLSupport
        Computer
        Core
        ${LLVM_PLUGIN}
        RICORDOSupport
    OPENCOR_BINARY_DEPENDENCIES
        ${LLVM_BINARY_PLUGIN}
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtNetwork
    EXTERNAL_DEPENDENCIES
        ${CELLML_API_EXTERNAL_DEPENDENCIES}
)
