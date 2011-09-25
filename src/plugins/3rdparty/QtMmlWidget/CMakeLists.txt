PROJECT(QtMmlWidgetPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/common.cmake)

# Add the plugin

ADD_PLUGIN(QtMmlWidget
    SOURCES
        ../../coreinterface.cpp
        ../../plugininfo.cpp

        src/qtmmlwidget.cpp
        src/qtmmlwidgetplugin.cpp
    HEADERS
        ../../coreinterface.h
        ../../plugininfo.h

        src/qtmmlwidget.h
    HEADERS_MOC
        src/qtmmlwidgetplugin.h
    PLUGIN_INCLUDE_DIRS
        include
        src
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtXml
)
