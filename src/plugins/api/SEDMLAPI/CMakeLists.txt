PROJECT(SEDMLAPIPlugin)

# Add the plugin

ADD_PLUGIN(SEDMLAPI
    SOURCES
        ../../plugininfo.cpp

        src/sedmlapiplugin.cpp
    HEADERS_MOC
        src/sedmlapiplugin.h
    QT_MODULES
        Core
    QT_LIBRARIES
        QtCore
)
