PROJECT(CellMLToolsPlugin)

# Retrieve the CellML API external binaries

RETRIEVE_CELLML_API_EXTERNAL_BINARIES()

# Apply LLVM's build settings

APPLY_LLVM_BUILD_SETTINGS()

# Add the plugin

ADD_PLUGIN(CellMLTools
    SOURCES
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../filetypeinterface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../pluginmanager.cpp

        src/cellmltoolsplugin.cpp
    HEADERS_MOC
        ../../plugin.h
        ../../pluginmanager.h

        src/cellmltoolsplugin.h
    DEFINITIONS
        ${LLVM_DEFINITIONS}
    INCLUDE_DIRS
        src
    PLUGINS
        CellMLAPI
        CellMLSupport
        Compiler
        Core
        ${LLVM_PLUGIN}
    PLUGIN_BINARIES
        ${LLVM_PLUGIN_BINARY}
    EXTERNAL_BINARIES
        ${CELLML_API_EXTERNAL_BINARIES}
    TESTS
        tests
)
