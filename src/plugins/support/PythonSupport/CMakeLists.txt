PROJECT(PythonSupportPlugin)

# Add the plugin

ADD_PLUGIN(PythonSupport
    SOURCES
        ../../plugin.cpp
        ../../pluginmanager.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp

        src/CTK/ctkAbstractPythonManager.cpp
        src/pythonsupport.cpp
        src/pythonsupportplugin.cpp

    HEADERS_MOC
        ../../plugin.h
        ../../pluginmanager.h

        src/CTK/ctkAbstractPythonManager.h
        src/pythonsupportplugin.h
    PLUGINS
        Python
        PythonQtAPI
)
