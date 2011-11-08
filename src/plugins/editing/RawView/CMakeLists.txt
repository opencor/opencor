PROJECT(RawViewPlugin)

# Add the plugin

ADD_PLUGIN(RawView
    SOURCES
        ../../coreinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/rawviewplugin.cpp
    HEADERS
        ../../coreinterface.h
        ../../guiinterface.h
        ../../i18ninterface.h
        ../../plugininfo.h
    HEADERS_MOC
        src/rawviewplugin.h
    INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        Core
    QT_DEPENDENCIES
        QtCore
        QtGui
)
