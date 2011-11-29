PROJECT(RawCellMLViewPlugin)

# Add the plugin

ADD_PLUGIN(RawCellMLView
    SOURCES
        ../../coreinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/rawcellmlviewplugin.cpp
    HEADERS_MOC
        src/rawcellmlviewplugin.h
    INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        Core
    QT_DEPENDENCIES
        QtCore
        QtGui
)
