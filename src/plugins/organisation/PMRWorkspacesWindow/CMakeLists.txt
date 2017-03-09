PROJECT(PMRWorkspacesWindowPlugin)

# Add the plugin

ADD_PLUGIN(PMRWorkspacesWindow
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
    HEADERS_MOC
        ../../preferencesinterface.h

        src/pmrworkspaceswindownewworkspacedialog.h
        src/pmrworkspaceswindowplugin.h
        src/pmrworkspaceswindowsynchronizedialog.h
        src/pmrworkspaceswindowwidget.h
        src/pmrworkspaceswindowwindow.h
    UIS
        src/pmrworkspaceswindownewworkspacedialog.ui
        src/pmrworkspaceswindowwindow.ui
    INCLUDE_DIRS
        src
    PLUGINS
        CellMLSupport
        PMRSupport
)
