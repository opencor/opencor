PROJECT(PythonConsoleWindowPlugin)

# Add the plugin

ADD_PLUGIN(PythonConsoleWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../windowinterface.cpp

        CTK/ctkConsole.cpp
        CTK/ctkPythonConsole.cpp
        src/pythonconsolewindowplugin.cpp
        src/pythonconsolewindow.cpp

    HEADERS_MOC
        CTK/ctkConsole.h
        CTK/ctkConsole_p.h
        CTK/ctkPythonConsole.h
        src/pythonconsolewindowplugin.h
        src/pythonconsolewindow.h
    UIS
        src/pythonconsolewindow.ui
    INCLUDE_DIRS
        CTK
        src
    PLUGINS
        Core
        PythonQtSupport
        PythonWrapper
)

#  ctkAbstractPythonManager.cpp
#  ctkAbstractPythonManager.h
