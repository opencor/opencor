PROJECT(CellMLToolsPlugin)

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
