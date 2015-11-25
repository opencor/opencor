PROJECT(PhysiomeModelRepositoryWindowPlugin)

# Apply libgit2's and zlib's build settings

APPLY_LIBGIT2_BUILD_SETTINGS()
APPLY_ZLIB_BUILD_SETTINGS()

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
)
