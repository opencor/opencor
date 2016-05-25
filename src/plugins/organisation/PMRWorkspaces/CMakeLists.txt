PROJECT(PMRWorkspacesPlugin)

# Add the plugin

ADD_PLUGIN(PMRWorkspaces
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/pmrworkspacesmodel.cpp
        src/pmrworkspacesplugin.cpp
        src/pmrworkspaceswidget.cpp
        src/pmrworkspaceswindow.cpp
    HEADERS_MOC
        src/pmrworkspacesmodel.h
        src/pmrworkspacesplugin.h
        src/pmrworkspaceswidget.h
        src/pmrworkspaceswindow.h
    UIS
        src/pmrworkspaceswindow.ui
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        ${LIBGIT2_PLUGIN}
        PMRSupport
    PLUGIN_BINARIES
        ${LIBGIT2_PLUGIN_BINARY}
        ${ZLIB_PLUGIN_BINARY}
)
