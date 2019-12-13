PROJECT(PythonSupportPlugin)

# Add the plugin

ADD_PLUGIN(PythonSupport
    SOURCES
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../pluginmanager.cpp

        src/pythonsupportplugin.cpp
    PLUGINS
        Core
        Python
        PythonPackages
    TESTS
        basictests
        coveragetests
        hodgkinhuxley1952tests
        importtests
        noble1962tests
        vanderpol1928tests
)
