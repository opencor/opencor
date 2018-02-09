project(CellMLToolsPlugin)

# Add the plugin

add_plugin(CellMLTools
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
    PLUGINS
        CellMLSupport
    TESTS
        tests
)
