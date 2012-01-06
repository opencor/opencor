PROJECT(CompilerPlugin)

# Retrieve some LLVM settings

RETRIEVE_LLVM_SETTINGS()

# Add the plugin

ADD_PLUGIN(Compiler
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/compilerengine.cpp
        src/compilerplugin.cpp
        src/compilerscanner.cpp
    HEADERS_MOC
        src/compilerengine.h
        src/compilerplugin.h
    DEFINITIONS
        ${LLVM_DEFINITIONS}
    INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        LLVM
    QT_DEPENDENCIES
        QtCore
        QtGui
    EXTERNAL_DEPENDENCIES
        ${LLVM_EXTERNAL_DEPENDENCIES}
)
