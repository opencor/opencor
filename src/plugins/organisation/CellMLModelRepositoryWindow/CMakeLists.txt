PROJECT(CellMLModelRepositoryWindowPlugin)

# Retrieve some libgit2 settings

RETRIEVE_LIBGIT2_SETTINGS()

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
        src/cellmlmodelrepositorywindowwidget.ui
        src/cellmlmodelrepositorywindowwindow.ui
    INCLUDE_DIRS
        src
    DEFINITIONS
        ${LIBGIT2_DEFINITIONS}
    PLUGINS
        Core
        ${LIBGIT2_PLUGIN}
    PLUGIN_BINARIES
        ${LIBGIT2_PLUGIN_BINARY}
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
