project(PythonPlugin)

# Add the plugin

add_plugin(Python
    SOURCES
        ../../cliinterface.cpp
        ../../plugininfo.cpp

        src/pythonplugin.cpp
    PLUGINS
        Core
        PythonQtSupport
)
