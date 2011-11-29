PROJECT(CellMLModelRepositoryPlugin)

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
        QJson
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtNetwork
        QtWebKit
)
