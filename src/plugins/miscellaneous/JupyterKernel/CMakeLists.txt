project(JupyterKernelPlugin)

# Add the plugin

add_plugin(JupyterKernel
    SOURCES
        ../../cliinterface.cpp
        ../../plugininfo.cpp

        src/jupyterkernelplugin.cpp
    PLUGINS
        Core
        PythonQtSupport
)

# Configure the OpenCOR Jupyter kernel and put it in its proper place

if(WIN32)
    set(OPENCOR_COMMAND ${CMAKE_PROJECT_NAME}.com)
else()
    set(OPENCOR_COMMAND ${CMAKE_PROJECT_NAME})
endif()

set(OPENCOR_KERNEL "${PROJECT_BUILD_DIR}/kernel.json")

configure_file(${PROJECT_SOURCE_DIR}/kernels/OpenCOR/kernel.json.in ${OPENCOR_KERNEL})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy ${OPENCOR_KERNEL}
                                                    ${PYTHON_ROOT_DIR}/share/jupyter/kernels/OpenCOR/kernel.json)
