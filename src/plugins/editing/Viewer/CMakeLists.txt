PROJECT(ViewerPlugin)

# Add the plugin

ADD_PLUGIN(Viewer
    SOURCES
        ../../plugininfo.cpp

        src/mmlviewerwidget.cpp
        src/viewerplugin.cpp
    HEADERS_MOC
        src/mmlviewerwidget.h
        src/viewerplugin.h
    INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        Core
        QtMmlWidget
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtXml
)
