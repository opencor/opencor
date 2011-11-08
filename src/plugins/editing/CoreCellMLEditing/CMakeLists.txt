PROJECT(CoreCellMLEditingPlugin)

# Add the plugin

ADD_PLUGIN(CoreCellMLEditing
    SOURCES
        ../../coreinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/corecellmleditingplugin.cpp
    HEADERS
        ../../coreinterface.h
        ../../guiinterface.h
        ../../i18ninterface.h
        ../../plugininfo.h
    HEADERS_MOC
        src/corecellmleditingplugin.h
    INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        Core
    QT_DEPENDENCIES
        QtCore
        QtGui
)
