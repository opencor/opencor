PROJECT(PythonConsoleWindowPlugin)

# Add the plugin

ADD_PLUGIN(PythonConsoleWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../windowinterface.cpp

        src/pythonconsolewindow.cpp
        src/pythonconsolewindowplugin.cpp
    UIS
        src/pythonconsolewindow.ui
    PLUGINS
        Core
        PythonQtSupport
)
