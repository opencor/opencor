PROJECT(CellMLToolsPlugin)

# Add the plugin

ADD_PLUGIN(CellMLTools
    SOURCES
        ../../cliinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../pluginmanager.cpp

        src/cellmltoolsplugin.cpp
    HEADERS_MOC
        ../../plugin.h
        ../../pluginmanager.h

        src/cellmltoolsplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        CellMLSupport
    TESTS
        tests
)
