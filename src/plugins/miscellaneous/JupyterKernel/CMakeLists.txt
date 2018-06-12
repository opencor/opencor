project(JupyterKernelPlugin)

# Add the plugin

add_plugin(JupyterKernel
    SOURCES
        ../../cliinterface.cpp
        ../../plugininfo.cpp

        src/jupyterkernelplugin.cpp
    HEADERS_MOC
        src/jupyterkernelplugin.h
    PLUGINS
        Core
        PythonQtSupport
)

# Copy the Jupyter kernel for OpenCOR to its proper place

add_custom_command(TARGET JupyterKernelPlugin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/kernels/OpenCOR
                                               ${PYTHON_ROOT_DIR}/share/jupyter/kernels/OpenCOR
    )
