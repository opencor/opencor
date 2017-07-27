PROJECT(JupyterKernelPlugin)

# Add the plugin

ADD_PLUGIN(JupyterKernel
    SOURCES
        ../../eventloopinterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp

        src/jupyterkernelplugin.cpp
    HEADERS_MOC
        src/jupyterkernelplugin.h
    PLUGINS
        Core
        PythonSupport
)
