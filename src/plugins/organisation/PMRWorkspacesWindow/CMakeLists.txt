project(PMRWorkspacesWindowPlugin)

# Add the plugin

add_plugin(PMRWorkspacesWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../preferencesinterface.cpp
        ../../windowinterface.cpp

        src/pmrworkspaceswindownewworkspacedialog.cpp
        src/pmrworkspaceswindowplugin.cpp
        src/pmrworkspaceswindowsynchronizedialog.cpp
        src/pmrworkspaceswindowwidget.cpp
        src/pmrworkspaceswindowwindow.cpp
    UIS
        src/pmrworkspaceswindownewworkspacedialog.ui
        src/pmrworkspaceswindowwindow.ui
    PLUGINS
        CellMLSupport
        LibXDiff
        PMRSupport
)
