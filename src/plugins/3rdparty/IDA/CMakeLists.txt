PROJECT(IDAPlugin)

# Add the plugin

ADD_PLUGIN(IDA
    SOURCES
        ../../plugininfo.cpp

        src/idaplugin.cpp
    HEADERS
        ../../plugininfo.h
    HEADERS_MOC
        src/idaplugin.h
    INCLUDE_DIRS
        src
    QT_DEPENDENCIES
        QtCore
)
