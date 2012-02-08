PROJECT(RICORDOSupportPlugin)

# Add the plugin

ADD_PLUGIN(RICORDOSupport
    SOURCES
        ../../fileinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/ricordosupportplugin.cpp
    HEADERS_MOC
        src/ricordosupportplugin.h
    INCLUDE_DIRS
        src
    QT_DEPENDENCIES
        QtCore
        QtGui
)
