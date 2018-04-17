PROJECT(PythonQtSupportPlugin)

# Add the plugin

ADD_PLUGIN(PythonQtSupport
    SOURCES
        ../../plugin.cpp
        ../../pluginmanager.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp

        src/CTK/ctkAbstractPythonManager.cpp
        src/pythonqtsupport.cpp
        src/pythonqtsupportplugin.cpp

    HEADERS_MOC
        ../../plugin.h
        ../../pluginmanager.h

        src/CTK/ctkAbstractPythonManager.h
        src/pythonqtsupportplugin.h
    PLUGINS
        PythonSupport
        PythonQtAPI
)
