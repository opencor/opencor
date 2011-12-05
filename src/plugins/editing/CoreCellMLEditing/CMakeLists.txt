PROJECT(CoreCellMLEditingPlugin)

# Add the plugin

ADD_PLUGIN(CoreCellMLEditing
    SOURCES
        ../../coreinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/corecellmleditingplugin.cpp
    HEADERS_MOC
        src/corecellmleditingplugin.h
    INCLUDE_DIRS
        src
    QT_DEPENDENCIES
        QtCore
        QtGui
)
