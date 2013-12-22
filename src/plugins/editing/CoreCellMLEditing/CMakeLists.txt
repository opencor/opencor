PROJECT(CoreCellMLEditingPlugin)

# Add the plugin

ADD_PLUGIN(CoreCellMLEditing
    SOURCES
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../pluginmanager.cpp

        src/corecellmleditingplugin.cpp
    HEADERS_MOC
        ../../plugin.h
        ../../pluginmanager.h

        src/corecellmleditingplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        Core
    QT_MODULES
        Widgets
    QT_LIBRARIES
        QtCore
        QtGui
        QtNetwork
        QtWidgets
)
