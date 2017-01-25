PROJECT(PythonWrapperPlugin)

# Our Python interpreter at buold time

set(PYTHON_EXECUTABLE ${PYTHON_ROOT_DIR}/bin/python)

# Find our installed NumPy

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
                      ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(NumPy REQUIRED)

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
        PythonQtSupport
        SingleCellView
)
