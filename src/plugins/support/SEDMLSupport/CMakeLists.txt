PROJECT(SEDMLSupportPlugin)

# Add the plugin

ADD_PLUGIN(SEDMLSupport
    SOURCES
        ../../filetypeinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp

        src/sedmlfile.cpp
        src/sedmlfileissue.cpp
        src/sedmlfilemanager.cpp
        src/sedmlsupportplugin.cpp
    HEADERS_MOC
        src/sedmlfile.h
        src/sedmlfilemanager.h
        src/sedmlsupportplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        CellMLAPI
        CellMLSupport
        Core
        ${LLVM_PLUGIN}
        SBMLAPI
        SEDMLAPI
        StandardSupport
    PLUGIN_BINARIES
        ${LLVM_PLUGIN_BINARY}
)
