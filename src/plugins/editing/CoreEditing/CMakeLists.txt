PROJECT(CoreEditingPlugin)

# Add the plugin

ADD_PLUGIN(CoreEditing
    SOURCES
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/coreeditingplugin.cpp
    HEADERS_MOC
        src/coreeditingplugin.h
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
