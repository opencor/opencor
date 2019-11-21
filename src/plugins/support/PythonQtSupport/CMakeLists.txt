PROJECT(PythonQtSupportPlugin)

# Add the plugin

ADD_PLUGIN(PythonQtSupport
    SOURCES
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../pluginmanager.cpp

        src/pythonqtsupport.cpp
        src/pythonqtsupportplugin.cpp
    PLUGINS
        PythonQt
        PythonSupport
)
