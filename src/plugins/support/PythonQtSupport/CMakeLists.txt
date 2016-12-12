PROJECT(PythonQtSupportPlugin)

# Add the plugin

ADD_PLUGIN(PythonConsoleWindow
    SOURCES
        ../../plugininfo.cpp
        ../../plugininterface.cpp

        CTK/ctkAbstractPythonManager.cpp
        src/pythonqtsupportplugin.cpp

    HEADERS_MOC
        CTK/ctkAbstractPythonManager.h
        src/pythonqtsupportplugin.h
    INCLUDE_DIRS
        CTK
        src
    PLUGINS
        PythonQtAPI
)
