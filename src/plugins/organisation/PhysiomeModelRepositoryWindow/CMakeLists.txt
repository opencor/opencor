PROJECT(PhysiomeModelRepositoryWindowPlugin)

# Retrieve some libgit2 and zlib settings

RETRIEVE_LIBGIT2_SETTINGS()
RETRIEVE_ZLIB_SETTINGS()

# Add the plugin

ADD_PLUGIN(PhysiomeModelRepositoryWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/physiomemodelrepositorywindowplugin.cpp
        src/physiomemodelrepositorywindowwidget.cpp
        src/physiomemodelrepositorywindowwindow.cpp
    HEADERS_MOC
        src/physiomemodelrepositorywindowplugin.h
        src/physiomemodelrepositorywindowwidget.h
        src/physiomemodelrepositorywindowwindow.h
    UIS
        src/physiomemodelrepositorywindowwidget.ui
        src/physiomemodelrepositorywindowwindow.ui
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
    QT_MODULES
        WebKitWidgets
)
