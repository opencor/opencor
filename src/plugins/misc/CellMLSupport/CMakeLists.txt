PROJECT(CellMLSupportPlugin)

# Retrieve some CellML API and LLVM settings

RETRIEVE_CELLML_API_SETTINGS()
RETRIEVE_LLVM_SETTINGS()

# Add the plugin

ADD_PLUGIN(CellMLSupport
    SOURCES
        ../../coreinterface.cpp
        ../../fileinterface.cpp
        ../../i18ninterface.cpp
        ../../interface.cpp
        ../../plugininfo.cpp

        src/cellmlfile.cpp
        src/cellmlfilecomponent.cpp
        src/cellmlfilecomponentref.cpp
        src/cellmlfileconnection.cpp
        src/cellmlfileelement.cpp
        src/cellmlfilegroup.cpp
        src/cellmlfileimport.cpp
        src/cellmlfileimportcomponent.cpp
        src/cellmlfileimportunit.cpp
        src/cellmlfileissue.cpp
        src/cellmlfilemanager.cpp
        src/cellmlfilemapcomponents.cpp
        src/cellmlfilemapvariables.cpp
        src/cellmlfilemathmlelement.cpp
        src/cellmlfilemodel.cpp
        src/cellmlfilenamedelement.cpp
        src/cellmlfilerdftriple.cpp
        src/cellmlfilerelationshipref.cpp
        src/cellmlfileruntime.cpp
        src/cellmlfileunit.cpp
        src/cellmlfileunitelement.cpp
        src/cellmlfilevariable.cpp
        src/cellmlsupportplugin.cpp
    HEADERS_MOC
        src/cellmlfile.h
        src/cellmlfilemanager.h
        src/cellmlfileruntime.h
        src/cellmlsupportplugin.h
    DEFINITIONS
        ${LLVM_DEFINITIONS}
    INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        CellMLAPI
        Computer
        Core
        ${LLVM_PLUGIN}
    OPENCOR_BINARY_DEPENDENCIES
        ${LLVM_BINARY_PLUGIN}
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtNetwork
    EXTERNAL_DEPENDENCIES
        ${CELLML_API_EXTERNAL_DEPENDENCIES}
)
