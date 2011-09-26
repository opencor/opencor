PROJECT(FileOrganiserPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/common.cmake)

# Add the plugin

ADD_PLUGIN(FileOrganiser
    SOURCES
        ../../coreinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/fileorganiserplugin.cpp
        src/fileorganiserwidget.cpp
        src/fileorganiserwindow.cpp
    HEADERS
        ../../coreinterface.h
        ../../guiinterface.h
        ../../i18ninterface.h
        ../../plugininfo.h
    HEADERS_MOC
        src/fileorganiserplugin.h
        src/fileorganiserwidget.h
        src/fileorganiserwindow.h
    UIS
        src/fileorganiserwindow.ui
    PLUGIN_INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        Core
    QT_DEPENDENCIES
        QtCore
        QtGui
)
