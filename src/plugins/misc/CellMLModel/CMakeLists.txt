PROJECT(CellMLModelPlugin)

# Add the plugin

ADD_PLUGIN(CellMLModel
    SOURCES
        ../../plugininfo.cpp

        src/cellmlmodelplugin.cpp
    HEADERS_MOC
        src/cellmlmodelplugin.h
    INCLUDE_DIRS
        src
    QT_DEPENDENCIES
        QtCore
)
