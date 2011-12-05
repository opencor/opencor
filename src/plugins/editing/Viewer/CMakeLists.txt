PROJECT(ViewerPlugin)

# Add the plugin

ADD_PLUGIN(Viewer
    SOURCES
        ../../coreinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/mmlviewerwidget.cpp
        src/viewerplugin.cpp
        src/viewerwindow.cpp
    HEADERS_MOC
        src/mmlviewerwidget.h
        src/viewerplugin.h
        src/viewerwindow.h
    UIS
        src/viewerwindow.ui
    INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        Core
        CoreEditing
        QtMmlWidget
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtXml
)
