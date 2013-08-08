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
    DEFINITIONS
        QWT_DLL
    INCLUDE_DIRS
        src
    PLUGIN_DEPENDENCIES
        Core
        Qwt
    QT_MODULES
        Widgets
    QT_DEPENDENCIES
        QtConcurrent
        QtCore
        QtGui
        QtNetwork
        QtOpenGL
        QtPrintSupport
        QtSvg
        QtWidgets
        QtXml
)
