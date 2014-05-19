PROJECT(SampleWindowPlugin)

# Add the plugin

ADD_PLUGIN(SampleWindow
    SOURCES
        ../../plugininfo.cpp

        src/samplewindowplugin.cpp
    HEADERS_MOC
        src/samplewindowplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        Sample
    QT_MODULES
        Widgets
    QT_LIBRARIES
        QtCore
        QtGui
        QtWidgets
)
