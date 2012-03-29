PROJECT(CorePlugin)

# Add the plugin

ADD_PLUGIN(Core
    SOURCES
        ../../coreinterface.cpp
        ../../fileinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../interface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp

        src/borderedwidget.cpp
        src/centralwidget.cpp
        src/commonwidget.cpp
        src/coreplugin.cpp
        src/coreutils.cpp
        src/dockwidget.cpp
        src/file.cpp
        src/filemanager.cpp
        src/organisationwidget.cpp
        src/toolbar.cpp
        src/treeview.cpp
        src/widget.cpp
    HEADERS_MOC
        ../../plugin.h

        src/borderedwidget.h
        src/centralwidget.h
        src/coreplugin.h
        src/dockwidget.h
        src/filemanager.h
        src/organisationwidget.h
        src/toolbar.h
        src/treeview.h
        src/widget.h
    UIS
        src/centralwidget.ui
    INCLUDE_DIRS
        src
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtNetwork
)
