project(CompilerPlugin)

# Add the plugin

add_plugin(Compiler
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/compilerengine.cpp
        src/compilermath.cpp
        src/compilerplugin.cpp
    HEADERS_MOC
        src/compilerengine.h
        src/compilerplugin.h
    PLUGINS
        Core
        LLVMClang
    TESTS
        tests
)
