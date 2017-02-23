PROJECT(PythonWrapperPlugin)

# Add the plugin

ADD_PLUGIN(PythonWrapper
    SOURCES
        ../../datastoreinterface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../pluginmanager.cpp

        src/pythonwrapperdatastore.cpp
        src/pythonwrapperplugin.cpp
        src/pythonwrappersinglecellview.cpp
    HEADERS_MOC
        ../../datastoreinterface.h
        ../../plugin.h
        ../../pluginmanager.h

        src/pythonwrapperdatastore.h
        src/pythonwrapperplugin.h
        src/pythonwrappersinglecellview.h
    INCLUDE_DIRS
        src
        ${NUMPY_INCLUDE_DIR}
    PLUGINS
        Core
        Python
        PythonQtSupport
        SingleCellView
)
