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

        src/cellmlannotationviewplugin.cpp
        src/cellmlannotationviewwidget.cpp
    HEADERS_MOC
        src/cellmlannotationviewplugin.h
        src/cellmlannotationviewwidget.h
    UIS
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
        LLVM
        RICORDOSupport
    QT_DEPENDENCIES
        QtCore
        QtGui
    EXTERNAL_DEPENDENCIES
        ${CELLML_API_EXTERNAL_DEPENDENCIES}
)
