PROJECT(SEDMLSupportPlugin)

# Retrieve the CellML, SBML and SED-ML APIs' external binaries

RETRIEVE_CELLML_API_EXTERNAL_BINARIES()
RETRIEVE_SBML_API_EXTERNAL_BINARIES()
RETRIEVE_SEDML_API_EXTERNAL_BINARIES()

# Apply LLVM's build settings

APPLY_LLVM_BUILD_SETTINGS()

# Add the plugin

ADD_PLUGIN(SEDMLSupport
    SOURCES
        ../../filetypeinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

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
    DEFINITIONS
        ${LLVM_DEFINITIONS}
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
    EXTERNAL_BINARIES
        ${CELLML_API_EXTERNAL_BINARIES}
        ${SBML_API_EXTERNAL_BINARIES}
        ${SEDML_API_EXTERNAL_BINARIES}
)
