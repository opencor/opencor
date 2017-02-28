PROJECT(RawTextViewPlugin)

# Add the plugin

ADD_PLUGIN(RawTextView
    SOURCES
        ../../filehandlinginterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../viewinterface.cpp

        ../EditingView/src/editingviewinterface.cpp

        src/rawtextviewplugin.cpp
        src/rawtextviewwidget.cpp
    HEADERS_MOC
        src/rawtextviewplugin.h
        src/rawtextviewwidget.h
    INCLUDE_DIRS
        src
    PLUGINS
        EditingView
)
