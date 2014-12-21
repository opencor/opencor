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
        Core
        ${LLVM_PLUGIN}
    PLUGIN_BINARIES
        ${LLVM_PLUGIN_BINARY}
    QT_MODULES
        Widgets
    QT_LIBRARIES
        QtCore
        QtGui
        QtNetwork
        QtXml
        QtXmlPatterns
        QtWidgets
    TESTS
        tests
)
