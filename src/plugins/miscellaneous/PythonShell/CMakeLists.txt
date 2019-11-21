project(PythonShellPlugin)

# Add the plugin

add_plugin(PythonShell
    SOURCES
        ../../cliinterface.cpp
        ../../plugininfo.cpp

        src/pythonshellplugin.cpp
    PLUGINS
        Core
        PythonQtSupport
)
