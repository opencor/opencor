PROJECT(CorePlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/common.cmake)

# Add the plugin

ADD_PLUGIN(Core
    SOURCES
        ../../apiinterface.cpp
        ../../coreinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp

        src/centralwidget.cpp
        src/commonwidget.cpp
        src/coreplugin.cpp
        src/coreutils.cpp
        src/docktoolbar.cpp
        src/dockwidget.cpp
        src/filemanager.cpp
        src/treeview.cpp
    HEADERS
        ../../apiinterface.h
        ../../coreinterface.h
        ../../guiinterface.h
        ../../i18ninterface.h
        ../../plugininfo.h

        src/commonwidget.h
        src/coreglobal.h
        src/coreutils.h
    HEADERS_MOC
        ../../plugin.h

        src/centralwidget.h
        src/coreplugin.h
        src/docktoolbar.h
        src/dockwidget.h
        src/filemanager.h
        src/treeview.h
    UIS
        src/centralwidget.ui
    PLUGIN_INCLUDE_DIRS
        src
    QT_DEPENDENCIES
        QtCore
        QtGui
)
