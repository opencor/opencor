PROJECT(PythonSupportPlugin)

# Add the plugin

ADD_PLUGIN(PythonSupport
    SOURCES
        ../../plugininfo.cpp
        ../../plugininterface.cpp

        src/CTK/ctkAbstractPythonManager.cpp
        src/pythonsupport.cpp
        src/pythonsupportplugin.cpp

    HEADERS_MOC
        src/CTK/ctkAbstractPythonManager.h
        src/pythonsupportplugin.h
    PLUGINS
        PythonQtAPI
)
