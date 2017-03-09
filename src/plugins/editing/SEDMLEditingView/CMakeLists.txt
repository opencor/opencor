PROJECT(SEDMLEditingViewPlugin)

# Add the plugin

ADD_PLUGIN(SEDMLEditingView
    SOURCES
        ../../filehandlinginterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../pluginmanager.cpp

        src/sedmleditingviewinterface.cpp
        src/sedmleditingviewplugin.cpp
        src/sedmleditingviewwidget.cpp
    HEADERS_MOC
        ../../plugin.h
        ../../pluginmanager.h

        src/sedmleditingviewplugin.h
        src/sedmleditingviewwidget.h
    INCLUDE_DIRS
        src
    PLUGINS
        EditingView
        SEDMLSupport
)
