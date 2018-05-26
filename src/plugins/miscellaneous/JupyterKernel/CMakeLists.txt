project(JupyterKernelPlugin)

# Add the plugin

add_plugin(JupyterKernel
    SOURCES
        ../../cliinterface.cpp
        ../../eventloopinterface.cpp
        ../../plugininfo.cpp

        src/jupyterkernelplugin.cpp
    HEADERS_MOC
        src/jupyterkernelplugin.h
    PLUGINS
        Core
        PythonQtSupport
)
