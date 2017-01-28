PROJECT(PythonWrapperPlugin)

# Add the plugin

ADD_PLUGIN(PythonWrapper
    SOURCES
        ../../plugininfo.cpp
        ../../plugininterface.cpp

        src/pythonwrapperdatastore.cpp
        src/pythonwrapperplugin.cpp
        src/pythonwrappersinglecellview.cpp
    HEADERS_MOC
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
