PROJECT(PythonQtSupportPlugin)

# Add the plugin

ADD_PLUGIN(PythonQtSupport
    SOURCES
        ../../plugininfo.cpp
        ../../plugininterface.cpp

        src/CTK/ctkAbstractPythonManager.cpp
        src/pythonqtsupportplugin.cpp

    HEADERS_MOC
        src/CTK/ctkAbstractPythonManager.h
        src/pythonqtsupportplugin.h
    INCLUDE_DIRS
    PLUGINS
        PythonQtAPI
)
