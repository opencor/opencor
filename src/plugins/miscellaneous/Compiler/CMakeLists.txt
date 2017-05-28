PROJECT(CompilerPlugin)

# Add the plugin

ADD_PLUGIN(Compiler
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/compilerengine.cpp
        src/compilermath.cpp
        src/compilerplugin.cpp
    HEADERS_MOC
        src/compilerplugin.h
    PLUGINS
        Core
        LLVMClang
    TESTS
        tests
)
