PROJECT(PMRWindowPlugin)

# Retrieve libgit2's and zlib's definitions

RETRIEVE_LIBGIT2_DEFINITIONS()
RETRIEVE_ZLIB_DEFINITIONS()

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
        src/pmrwindowwidget.ui
        src/pmrwindowwindow.ui
    INCLUDE_DIRS
        src
    DEFINITIONS
        ${LIBGIT2_DEFINITIONS}
        ${ZLIB_DEFINITIONS}
    PLUGINS
        Core
        ${LIBGIT2_PLUGIN}
        ${ZLIB_PLUGIN}
    PLUGIN_BINARIES
        ${LIBGIT2_PLUGIN_BINARY}
        ${ZLIB_PLUGIN_BINARY}
)
