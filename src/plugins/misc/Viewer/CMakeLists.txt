PROJECT(ViewerPlugin)

# Add the plugin

ADD_PLUGIN(Viewer
    SOURCES
        ../../plugininfo.cpp

        src/viewerwidget.cpp
        src/viewerplugin.cpp
    HEADERS_MOC
        src/viewerwidget.h
        src/viewerplugin.h
    INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        Core
        QtMmlWidget
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtNetwork
        QtXml
)
