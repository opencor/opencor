project(CompilerPlugin)

# Add the plugin

add_plugin(Compiler
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/compilerengine.cpp
        src/compilermath.cpp
        src/compilerplugin.cpp
    PLUGINS
        Core
        LLVMClang
    TESTS
        tests
)
