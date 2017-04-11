PROJECT(PythonWrapperPlugin)

# Add the plugin

ADD_PLUGIN(PythonWrapper
    # Suppress warnings when using Python C API headers

    THIRD_PARTY

    SOURCES
        ../../datastoreinterface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../pluginmanager.cpp

        src/pythonwrappercore.cpp
        src/pythonwrapperdatastore.cpp
        src/pythonwrapperplugin.cpp
        src/pythonwrappersimulationexperimentview.cpp
    HEADERS_MOC
        ../../datastoreinterface.h
        ../../plugin.h
        ../../pluginmanager.h

        src/pythonwrappercore.h
        src/pythonwrapperdatastore.h
        src/pythonwrapperplugin.h
        src/pythonwrappersimulationexperimentview.h
    INCLUDE_DIRS
        src
        ${NUMPY_INCLUDE_DIR}
    PLUGINS
        Core
        Python
        PythonQtSupport
        SimulationExperimentView
)
