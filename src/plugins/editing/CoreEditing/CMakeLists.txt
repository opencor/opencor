PROJECT(CoreEditingPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/common.cmake)

# Add the plugin

ADD_PLUGIN(CoreEditing
    SOURCES
        ../../guiinterface.cpp
        ../../plugininterface.cpp

        src/coreeditingplugin.cpp
    HEADERS
        ../../guiinterface.h
        ../../plugininterface.h
    HEADERS_MOC
        src/coreeditingplugin.h
    PLUGIN_INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        Core
    QT_DEPENDENCIES
        QtCore
        QtGui
)
