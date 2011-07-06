PROJECT(ViewerPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/common.cmake)

# Add the plugin

ADD_PLUGIN(Viewer TRUE
    SOURCES
        ../../guiinterface.cpp
        ../../plugininterface.cpp

        src/mmlviewerwidget.cpp
        src/viewerplugin.cpp
        src/viewerwindow.cpp
    HEADERS
        ../../guiinterface.h
        ../../plugininterface.h
    HEADERS_MOC
        src/mmlviewerwidget.h
        src/viewerplugin.h
        src/viewerwindow.h
    UIS
        src/viewerwindow.ui
    PLUGIN_INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        Core
        QtMmlWidget
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtXml
)
