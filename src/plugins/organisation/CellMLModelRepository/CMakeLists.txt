PROJECT(CellMLModelRepositoryPlugin)

# Add the plugin

ADD_PLUGIN(CellMLModelRepository
    SOURCES
        ../../coreinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../interface.cpp
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
    OPENCOR_DEPENDENCIES
        Core
    QT_MODULES
        WebKitWidgets
        Widgets
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtNetwork
        QtOpenGL
        QtPrintSupport
        QtQml
        QtQuick
        QtWebKit
        QtWebKitWidgets
        QtWidgets
)
