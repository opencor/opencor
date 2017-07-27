PROJECT(PythonQtConsoleWindowPlugin)

# Add the plugin

ADD_PLUGIN(PythonQtConsoleWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../windowinterface.cpp

        src/pythonqtconsolewindowplugin.cpp
        src/pythonqtconsolewindow.cpp

    HEADERS_MOC
        src/pythonqtconsolewindowplugin.h
        src/pythonqtconsolewindow.h
    UIS
        src/pythonqtconsolewindow.ui
    PLUGINS
        Core
        PythonSupport
)
