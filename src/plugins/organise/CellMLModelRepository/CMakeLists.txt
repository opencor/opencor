PROJECT(CellMLModelRepositoryPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/plugin.cmake)

# Initialise the plugin

INITIALISE_PLUGIN(CellMLModelRepository TRUE)

# Files that make up the plugin

SET(SOURCES
    src/cellmlmodelrepositoryplugin.cpp
    src/cellmlmodelrepositorywidget.cpp
    src/cellmlmodelrepositorywindow.cpp
)

SET(HEADERS_MOC
    src/cellmlmodelrepositoryplugin.h
    src/cellmlmodelrepositorywidget.h
    src/cellmlmodelrepositorywindow.h
)

SET(UIS
    src/cellmlmodelrepositorywindow.ui
)

# Various include directories

SET(LOCAL_PLUGIN_INCLUDE_DIRS src)
SET(PLUGIN_INCLUDE_DIRS ${LOCAL_PLUGIN_INCLUDE_DIRS} PARENT_SCOPE)

INCLUDE_DIRECTORIES(${LOCAL_PLUGIN_INCLUDE_DIRS})

# Build the plugin

BUILD_PLUGIN(
    CorePlugin

    ${QT_QTWEBKIT_LIBRARY}
)

# Package the plugin

PACKAGE_PLUGIN()
