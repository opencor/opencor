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
        src
    QT_MODULES
        Widgets
        Xml
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtWidgets
        QtXml
)
