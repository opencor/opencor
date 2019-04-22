project(PMRSupportPlugin)

# Add the plugin

add_plugin(PMRSupport
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../preferencesinterface.cpp

        src/pmrauthentication.cpp
        src/pmrexposure.cpp
        src/pmrsupport.cpp
        src/pmrsupportplugin.cpp
        src/pmrsupportpreferenceswidget.cpp
        src/pmrwebservice.cpp
        src/pmrwebservicemanager.cpp
        src/pmrwebserviceresponse.cpp
        src/pmrwebviewerdialog.cpp
        src/pmrworkspace.cpp
        src/pmrworkspacefilenode.cpp
        src/pmrworkspacemanager.cpp
    UIS
        src/pmrsupportpreferenceswidget.ui
    PLUGINS
        Core
        libgit2
        OAuth
        WebViewerWidget
)
