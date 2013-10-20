PROJECT(CellMLModelRepositoryPlugin)

# Add the plugin

ADD_PLUGIN(CellMLModelRepository
    SOURCES
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/cellmlmodelrepositoryplugin.cpp
        src/cellmlmodelrepositorywidget.cpp
        src/cellmlmodelrepositorywindow.cpp
    HEADERS_MOC
        src/cellmlmodelrepositoryplugin.h
        src/cellmlmodelrepositorywidget.h
        src/cellmlmodelrepositorywindow.h
    UIS
        src/cellmlmodelrepositorywindow.ui
    INCLUDE_DIRS
        src
    PLUGIN_DEPENDENCIES
        Core
    QT_MODULES
        WebKitWidgets
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtNetwork
        QtOpenGL
        QtPrintSupport
        QtQml
        QtQuick
        QtSensors
        QtWebKit
        QtWebKitWidgets
        QtWidgets
)
