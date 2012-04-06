PROJECT(QtMmlWidgetPlugin)

# Add the plugin

ADD_PLUGIN(QtMmlWidget
    THIRD_PARTY
    SOURCES
        ../../plugininfo.cpp

        src/qtmmlwidget.cpp
        src/qtmmlwidgetplugin.cpp
    HEADERS_MOC
        src/qtmmlwidgetplugin.h
    INCLUDE_DIRS
        include
        src
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtXml
)
