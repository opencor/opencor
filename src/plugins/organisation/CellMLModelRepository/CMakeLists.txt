PROJECT(CellMLModelRepositoryPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/common.cmake)

# Add the plugin

ADD_PLUGIN(CellMLModelRepository
    SOURCES
        ../../coreinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/cellmlmodelrepositoryplugin.cpp
        src/cellmlmodelrepositorywidget.cpp
        src/cellmlmodelrepositorywindow.cpp
    HEADERS
        ../../coreinterface.h
        ../../guiinterface.h
        ../../i18ninterface.h
        ../../plugininfo.h
    HEADERS_MOC
        src/cellmlmodelrepositoryplugin.h
        src/cellmlmodelrepositorywidget.h
        src/cellmlmodelrepositorywindow.h
    UIS
        src/cellmlmodelrepositorywindow.ui
    PLUGIN_INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        Core
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtWebKit
)
