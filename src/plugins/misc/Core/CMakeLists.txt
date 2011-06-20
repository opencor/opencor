PROJECT(CorePlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/plugin.cmake)

# Initialise the plugin

INITIALISE_PLUGIN(Core TRUE)

# Files that make up the plugin

SET(SOURCES
    src/centralwidget.cpp
    src/checkforupdateswindow.cpp
    src/commonwidget.cpp
    src/coreutils.cpp
    src/docktoolbar.cpp
    src/dockwidget.cpp
    src/filemanager.cpp
    src/pluginswindow.cpp
    src/preferenceswindow.cpp
    src/tabwidget.cpp
    src/treeview.cpp
)

SET(HEADERS
    src/commonwidget.h
    src/coreutils.h
)

SET(HEADERS_MOC
    src/centralwidget.h
    src/checkforupdateswindow.h
    src/docktoolbar.h
    src/dockwidget.h
    src/filemanager.h
    src/pluginswindow.h
    src/preferenceswindow.h
    src/tabwidget.h
    src/treeview.h
)

SET(UIS
    src/centralwidget.ui
    src/checkforupdateswindow.ui
    src/pluginswindow.ui
    src/preferenceswindow.ui
)

# Various include directories

SET(LOCAL_PLUGIN_INCLUDE_DIRS src)
SET(PLUGIN_INCLUDE_DIRS ${LOCAL_PLUGIN_INCLUDE_DIRS} PARENT_SCOPE)

INCLUDE_DIRECTORIES(${LOCAL_PLUGIN_INCLUDE_DIRS})

# Build the plugin

BUILD_PLUGIN(
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
)

# Package the plugin

PACKAGE_PLUGIN()
