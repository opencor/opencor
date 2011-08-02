PROJECT(CellMLXMLViewPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/common.cmake)

# Add the plugin

ADD_PLUGIN(CellMLXMLView
    SOURCES
        ../../plugininterface.cpp

        src/cellmlxmlviewplugin.cpp
    HEADERS
        ../../plugininterface.h
    HEADERS_MOC
        src/cellmlxmlviewplugin.h
    PLUGIN_INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        Core
)
