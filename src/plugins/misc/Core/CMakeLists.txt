PROJECT(CorePlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/common.cmake)

# Add the plugin

ADD_PLUGIN(Core TRUE
    SOURCES
        ../../guiinterface.cpp
        ../../plugininterface.cpp

        src/centralwidget.cpp
        src/checkforupdateswindow.cpp
        src/commonwidget.cpp
        src/coreplugin.cpp
        src/coreutils.cpp
        src/docktoolbar.cpp
        src/dockwidget.cpp
        src/filemanager.cpp
        src/pluginswindow.cpp
        src/preferenceswindow.cpp
        src/tabwidget.cpp
        src/treeview.cpp
    HEADERS
        ../../guiinterface.h
        ../../plugininterface.h

        src/commonwidget.h
        src/coreglobal.h
        src/coreutils.h
    HEADERS_MOC
        src/centralwidget.h
        src/checkforupdateswindow.h
        src/coreplugin.h
        src/docktoolbar.h
        src/dockwidget.h
        src/filemanager.h
        src/pluginswindow.h
        src/preferenceswindow.h
        src/tabwidget.h
        src/treeview.h
    UIS
        src/centralwidget.ui
        src/checkforupdateswindow.ui
        src/pluginswindow.ui
        src/preferenceswindow.ui
    PLUGIN_INCLUDE_DIRS
        src
    QT_DEPENDENCIES
        QtCore
        QtGui
)
