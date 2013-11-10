PROJECT(CompilerPlugin)

# Retrieve some LLVM settings

RETRIEVE_LLVM_SETTINGS()

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
        ${LLVM_PLUGIN}
    PLUGIN_BINARIES
        ${LLVM_BINARY_PLUGIN}
    QT_MODULES
        Widgets
    QT_LIBRARIES
        QtCore
        QtGui
        QtWidgets
    TESTS
        tests
)
