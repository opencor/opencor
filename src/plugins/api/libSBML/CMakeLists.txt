PROJECT(libSBMLPlugin)

# Add the plugin

ADD_PLUGIN(libSBML
    SOURCES
        ../../plugininfo.cpp

        src/libsbmlplugin.cpp
    HEADERS_MOC
        src/libsbmlplugin.h
    QT_MODULES
        Core
    QT_LIBRARIES
        QtCore
)
