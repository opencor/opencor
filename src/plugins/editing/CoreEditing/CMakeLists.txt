PROJECT(CoreEditingPlugin)

# Add the plugin

ADD_PLUGIN(CoreEditing
    SOURCES
        ../../coreinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../interface.cpp
        ../../plugininfo.cpp

        src/coreeditingplugin.cpp
    HEADERS_MOC
        src/coreeditingplugin.h
    INCLUDE_DIRS
        src
    PLUGIN_DEPENDENCIES
        Core
    QT_MODULES
        Widgets
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtNetwork
        QtWidgets
)
