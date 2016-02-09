PROJECT(COMBINESupportPlugin)

# Retrieve the CellML API's external binaries

RETRIEVE_CELLML_API_EXTERNAL_BINARIES()

# Apply LLVM and zlib's build settings

APPLY_LLVM_BUILD_SETTINGS()
APPLY_ZLIB_BUILD_SETTINGS()

# Add the plugin

ADD_PLUGIN(COMBINESupport
    SOURCES
        ../../filetypeinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/combinearchive.cpp
        src/combinearchiveissue.cpp
        src/combinefilemanager.cpp
        src/combinesupportplugin.cpp
    HEADERS_MOC
        src/combinearchive.h
        src/combinefilemanager.h
        src/combinesupportplugin.h
    INCLUDE_DIRS
        src
    DEFINITIONS
        ${LLVM_DEFINITIONS}
        ${ZLIB_DEFINITIONS}
    PLUGINS
        CellMLAPI
        CellMLSupport
        Core
        ${LLVM_PLUGIN}
        StandardSupport
        ZIPSupport
        ${ZLIB_PLUGIN}
    PLUGIN_BINARIES
        ${LLVM_PLUGIN_BINARY}
        ${ZLIB_PLUGIN_BINARY}
    EXTERNAL_BINARIES
        ${CELLML_API_EXTERNAL_BINARIES}
    TESTS
        tests
)
