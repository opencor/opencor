PROJECT(SampleViewPlugin)

# Add the plugin

ADD_PLUGIN(SampleView
    SOURCES
        ../../plugininfo.cpp

        src/sampleviewplugin.cpp
    HEADERS_MOC
        src/sampleviewplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        Sample
    QT_MODULES
        Widgets
    QT_LIBRARIES
        QtCore
        QtGui
        QtNetwork
        QtWidgets
        QtXmlPatterns
)
