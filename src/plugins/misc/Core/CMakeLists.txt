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
        ../../pluginmanager.cpp

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
        src/usermessagewidget.cpp
        src/widget.cpp
    HEADERS_MOC
        ../../plugin.h
        ../../pluginmanager.h

        src/borderedwidget.h
        src/centralwidget.h
        src/coreplugin.h
        src/dockwidget.h
        src/filemanager.h
        src/organisationwidget.h
        src/toolbar.h
        src/treeview.h
        src/usermessagewidget.h
        src/widget.h
    UIS
        src/centralwidget.ui
        src/usermessagewidget.ui
    INCLUDE_DIRS
        src
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtNetwork
)
