PROJECT(PythonWrapperPlugin)

# Add the plugin

ADD_PLUGIN(PythonWrapper
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
        src/pythonwrappersolver.cpp
    HEADERS_MOC
        ../../datastoreinterface.h
        ../../plugin.h
        ../../pluginmanager.h

        src/pythonwrappercore.h
        src/pythonwrapperdatastore.h
        src/pythonwrapperplugin.h
        src/pythonwrappersimulationexperimentview.h
        src/pythonwrappersolver.h
    INCLUDE_DIRS
        ${PYTHON_INCLUDE_DIR}
        ${NUMPY_INCLUDE_DIR}
    PLUGINS
        Core
        PythonQtSupport
        SimulationExperimentView
    DEPENDS_ON
        PythonPackagesPlugin
)
