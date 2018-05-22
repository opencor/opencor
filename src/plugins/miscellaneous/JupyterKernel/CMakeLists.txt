project(JupyterKernelPlugin)

# Add the plugin

add_plugin(JupyterKernel
    SOURCES
        ../../eventloopinterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp

        src/jupyterkernelplugin.cpp
    HEADERS_MOC
        src/jupyterkernelplugin.h
    PLUGINS
        Core
        PythonQtSupport
)
