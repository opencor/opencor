PROJECT(RICORDOSupportPlugin)

# Add the plugin

ADD_PLUGIN(RICORDOSupport
    SOURCES
        ../../plugininfo.cpp

        src/ricordosupportplugin.cpp
    HEADERS_MOC
        src/ricordosupportplugin.h
    INCLUDE_DIRS
        src
    QT_MODULES
        Core
    QT_DEPENDENCIES
        QtCore
)
