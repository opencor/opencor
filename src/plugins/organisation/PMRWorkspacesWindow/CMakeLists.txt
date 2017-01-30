PROJECT(PMRWorkspacesWindowPlugin)

# Add the plugin

ADD_PLUGIN(PMRWorkspacesWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../windowinterface.cpp

        src/pmrworkspaceswindowcommitdialog.cpp
        src/pmrworkspaceswindowitem.cpp
        src/pmrworkspaceswindownewworkspacedialog.cpp
        src/pmrworkspaceswindowplugin.cpp
        src/pmrworkspaceswindowwidget.cpp
        src/pmrworkspaceswindowwindow.cpp
    HEADERS_MOC
        src/pmrworkspaceswindowcommitdialog.h
        src/pmrworkspaceswindownewworkspacedialog.h
        src/pmrworkspaceswindowplugin.h
        src/pmrworkspaceswindowwidget.h
        src/pmrworkspaceswindowwindow.h
    UIS
        src/pmrworkspaceswindowcommitdialog.ui
        src/pmrworkspaceswindownewworkspacedialog.ui
        src/pmrworkspaceswindowwindow.ui
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        ${LIBGIT2_PLUGIN}
        ${OAUTH_PLUGIN}
        PMRSupport
        ${ZLIB_PLUGIN}
    PLUGIN_BINARIES
        ${LIBGIT2_PLUGIN_BINARY}
        ${OAUTH_PLUGIN_BINARY}
        ${ZLIB_PLUGIN_BINARY}
)
