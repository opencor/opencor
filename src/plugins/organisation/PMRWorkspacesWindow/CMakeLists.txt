PROJECT(PMRWorkspacesWindowPlugin)

# Add the plugin

ADD_PLUGIN(PMRWorkspacesWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/pmrworkspaceswindowcommit.cpp
        src/pmrworkspaceswindownewworkspace.cpp
        src/pmrworkspaceswindowplugin.cpp
        src/pmrworkspaceswindowwidget.cpp
        src/pmrworkspaceswindowwindow.cpp
    HEADERS_MOC
        src/pmrworkspaceswindowcommit.h
        src/pmrworkspaceswindownewworkspace.h
        src/pmrworkspaceswindowplugin.h
        src/pmrworkspaceswindowwidget.h
        src/pmrworkspaceswindowwindow.h
    UIS
        src/pmrworkspaceswindowcommit.ui
        src/pmrworkspaceswindownewworkspace.ui
        src/pmrworkspaceswindowwindow.ui
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        ${LIBGIT2_PLUGIN}
        ${OAUTH_PLUGIN}
        PMRSupport
        WebViewerWidget
        ${ZLIB_PLUGIN}
    PLUGIN_BINARIES
        ${LIBGIT2_PLUGIN_BINARY}
        ${OAUTH_PLUGIN_BINARY}
        ${ZLIB_PLUGIN_BINARY}
)
