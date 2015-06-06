PROJECT(PhysiomeModelRepositoryWindowPlugin)

# Retrieve some libgit2 settings

RETRIEVE_LIBGIT2_SETTINGS()

# Add the plugin

ADD_PLUGIN(PhysiomeModelRepositoryWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/physiomemodelrepositorywindowplugin.cpp
        src/physiomemodelrepositorywindowwidget.cpp
        src/physiomemodelrepositorywindowwindow.cpp
    HEADERS_MOC
        src/physiomemodelrepositorywindowplugin.h
        src/physiomemodelrepositorywindowwidget.h
        src/physiomemodelrepositorywindowwindow.h
    UIS
        src/physiomemodelrepositorywindowwidget.ui
        src/physiomemodelrepositorywindowwindow.ui
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
