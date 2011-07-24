PROJECT(FileBrowserPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/common.cmake)

# Add the plugin

ADD_PLUGIN(FileBrowser
    SOURCES
        ../../guiinterface.cpp
        ../../plugininterface.cpp

        src/filebrowserplugin.cpp
        src/filebrowserwidget.cpp
        src/filebrowserwindow.cpp
    HEADERS
        ../../guiinterface.h
        ../../plugininterface.h
    HEADERS_MOC
        src/filebrowserplugin.h
        src/filebrowserwidget.h
        src/filebrowserwindow.h
    UIS
        src/filebrowserwindow.ui
    PLUGIN_INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        Core
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtSvg
)
