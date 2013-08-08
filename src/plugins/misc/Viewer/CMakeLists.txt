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
    PLUGIN_DEPENDENCIES
        Qwt
    QT_MODULES
        Widgets
    QT_DEPENDENCIES
        QtConcurrent
        QtCore
        QtGui
        QtOpenGL
        QtPrintSupport
        QtSvg
        QtWidgets
        QtXml
)
