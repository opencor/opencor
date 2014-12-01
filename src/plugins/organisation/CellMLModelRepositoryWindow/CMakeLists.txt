PROJECT(CellMLModelRepositoryWindowPlugin)

# Add the plugin

ADD_PLUGIN(CellMLModelRepositoryWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/cellmlmodelrepositorywindowplugin.cpp
        src/cellmlmodelrepositorywindowwidget.cpp
        src/cellmlmodelrepositorywindowwindow.cpp
    HEADERS_MOC
        src/cellmlmodelrepositorywindowplugin.h
        src/cellmlmodelrepositorywindowwidget.h
        src/cellmlmodelrepositorywindowwindow.h
    UIS
        src/cellmlmodelrepositorywindowwindow.ui
    INCLUDE_DIRS
        src
    PLUGINS
        Core
    QT_MODULES
        WebKitWidgets
    QT_LIBRARIES
        QtCore
        QtGui
        QtNetwork
        QtWebKit
        QtWebKitWidgets
        QtWidgets
        QtXml
        QtXmlPatterns
)
