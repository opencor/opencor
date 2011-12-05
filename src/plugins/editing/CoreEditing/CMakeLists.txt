PROJECT(CoreEditingPlugin)

# Add the plugin

ADD_PLUGIN(CoreEditing
    SOURCES
        ../../coreinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/coreeditingplugin.cpp
    HEADERS_MOC
        src/coreeditingplugin.h
    INCLUDE_DIRS
        src
    QT_DEPENDENCIES
        QtCore
        QtGui
)
