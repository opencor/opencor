PROJECT(CorePlugin)

# Add the plugin

ADD_PLUGIN(Core
    SOURCES
        ../../coreinterface.cpp
        ../../fileinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp

        src/centralwidget.cpp
        src/commonwidget.cpp
        src/coreplugin.cpp
        src/coreutils.cpp
        src/docktoolbar.cpp
        src/dockwidget.cpp
        src/filemanager.cpp
        src/organisationwidget.cpp
        src/treeview.cpp
    HEADERS_MOC
        ../../plugin.h

        src/centralwidget.h
        src/coreplugin.h
        src/docktoolbar.h
        src/dockwidget.h
        src/filemanager.h
        src/organisationwidget.h
        src/treeview.h
    UIS
        src/centralwidget.ui
    INCLUDE_DIRS
        src
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtNetwork
)
