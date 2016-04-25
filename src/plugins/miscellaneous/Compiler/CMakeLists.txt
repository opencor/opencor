PROJECT(CompilerPlugin)

# Retrieve LLVM's definitions

RETRIEVE_LLVM_DEFINITIONS()

# Add the plugin

ADD_PLUGIN(Compiler
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/compilerengine.cpp
        src/compilermath.cpp
        src/compilerplugin.cpp
    HEADERS_MOC
        src/compilerengine.h
        src/compilerplugin.h
    INCLUDE_DIRS
        src
    DEFINITIONS
        ${LLVM_DEFINITIONS}
    PLUGINS
        Core
        ${LLVM_PLUGIN}
    PLUGIN_BINARIES
        ${LLVM_PLUGIN_BINARY}
    TESTS
        tests
)
