PROJECT(EditingViewPlugin)

# Add the plugin

ADD_PLUGIN(EditingView
    SOURCES
        ../../filehandlinginterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../pluginmanager.cpp

        src/editingviewinterface.cpp
        src/editingviewplugin.cpp
    HEADERS_MOC
        ../../plugin.h
        ../../pluginmanager.h

        src/editingviewplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        EditorWidget
)
