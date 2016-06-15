PROJECT(PMRWindowPlugin)

# Add the plugin

ADD_PLUGIN(PMRWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/pmrwindowplugin.cpp
        src/pmrwindowwidget.cpp
        src/pmrwindowwindow.cpp
    HEADERS_MOC
        src/pmrwindowplugin.h
        src/pmrwindowwidget.h
        src/pmrwindowwindow.h
    UIS
        src/pmrwindowwindow.ui
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        ${LIBGIT2_PLUGIN}
        PMRSupport
        WebViewer
        ${ZLIB_PLUGIN}
    PLUGIN_BINARIES
        ${LIBGIT2_PLUGIN_BINARY}
        ${ZLIB_PLUGIN_BINARY}
)
