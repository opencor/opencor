PROJECT(QtMmlWidgetPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/common.cmake)

# Add the plugin

ADD_PLUGIN(QtMmlWidget
    SOURCES
        ../../plugininterface.cpp

        src/qtmmlwidget.cpp
        src/qtmmlwidgetplugin.cpp
    HEADERS
        ../../plugininterface.h

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
