PROJECT(PythonQtSupportPlugin)

# Add the plugin

ADD_PLUGIN(PythonConsoleWindow
    SOURCES
        ../../plugininfo.cpp
        ../../plugininterface.cpp

        src/pythonqtsupportplugin.cpp

    HEADERS_MOC
        src/pythonqtsupportplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        PythonQt
)

#  ctkAbstractPythonManager.cpp
#  ctkAbstractPythonManager.h
