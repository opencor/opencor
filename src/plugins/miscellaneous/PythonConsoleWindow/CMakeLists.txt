PROJECT(PythonConsoleWindowPlugin)

# Add the plugin

ADD_PLUGIN(PythonConsoleWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../windowinterface.cpp

        src/pythonconsolewindowplugin.cpp
        src/pythonconsolewindow.cpp
    UIS
        src/pythonconsolewindow.ui
    PLUGINS
        Core
        PythonQtSupport
)
