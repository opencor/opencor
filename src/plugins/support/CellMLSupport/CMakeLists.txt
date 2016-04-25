PROJECT(CellMLSupportPlugin)

# Add the plugin

ADD_PLUGIN(CellMLSupport
    SOURCES
        ../../filetypeinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../solverinterface.cpp

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
        ../../solverinterface.h

        src/cellmlfile.h
        src/cellmlfilemanager.h
        src/cellmlsupportplugin.h
    INCLUDE_DIRS
        src
    DEFINITIONS
        ${LLVM_DEFINITIONS}
    PLUGINS
        CellMLAPI
        Compiler
        Core
        ${LLVM_PLUGIN}
        StandardSupport
    PLUGIN_BINARIES
        ${LLVM_PLUGIN_BINARY}
    EXTERNAL_BINARIES
        ${CELLML_API_EXTERNAL_BINARIES}
)
