PROJECT(CoreCellMLEditingPlugin)

# Add the plugin

ADD_PLUGIN(CoreCellMLEditing
    SOURCES
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/corecellmleditingplugin.cpp
    HEADERS_MOC
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
