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
        src/cellmlfileissue.cpp
        src/cellmlfilemanager.cpp
        src/cellmlfilerdftriple.cpp
        src/cellmlfilerdftripleelement.cpp
        src/cellmlfileruntime.cpp
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
    PLUGIN_DEPENDENCIES
        CellMLAPI
        Compiler
        Core
        CoreSolver
        ${LLVM_PLUGIN}
    PLUGIN_BINARY_DEPENDENCIES
        ${LLVM_BINARY_PLUGIN}
    QT_MODULES
        Widgets
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtNetwork
        QtWidgets
    EXTERNAL_BINARY_DEPENDENCIES
        ${CELLML_API_EXTERNAL_BINARY_DEPENDENCIES}
)
