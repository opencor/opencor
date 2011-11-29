PROJECT(CVODEPlugin)

# Add the plugin

ADD_PLUGIN(CVODE
    SOURCES
        ../../plugininfo.cpp

        src/cvodeplugin.cpp
    HEADERS
        ../../plugininfo.h
    HEADERS_MOC
        src/cvodeplugin.h
    INCLUDE_DIRS
        src
    QT_DEPENDENCIES
        QtCore
)
