PROJECT(CellMLSupportPlugin)

# Retrieve some CellML API and LLVM settings

RETRIEVE_CELLML_API_SETTINGS()
RETRIEVE_LLVM_SETTINGS()

# Add the plugin

ADD_PLUGIN(CellMLSupport
    SOURCES
        ../../fileinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/cellmlfile.cpp
        src/cellmlfilecellml10exporter.cpp
        src/cellmlfilecellml11exporter.cpp
        src/cellmlfilecellmlexporter.cpp
        src/cellmlfileexporter.cpp
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
    PLUGINS
        CellMLAPI
        Compiler
        Core
        CoreSolver
        ${LLVM_PLUGIN}
    PLUGIN_BINARIES
        ${LLVM_BINARY_PLUGIN}
    QT_MODULES
        Widgets
    QT_LIBRARIES
        QtCore
        QtGui
        QtNetwork
        QtWidgets
    EXTERNAL_BINARIES
        ${CELLML_API_EXTERNAL_BINARIES}
)
