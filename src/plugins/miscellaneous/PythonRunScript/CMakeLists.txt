project(PythonRunScriptPlugin)

# Add the plugin

add_plugin(PythonRunScript
    SOURCES
        ../../cliinterface.cpp
        ../../plugininfo.cpp

        src/pythonrunscriptplugin.cpp
    PLUGINS
        Core
        PythonQtSupport
)
