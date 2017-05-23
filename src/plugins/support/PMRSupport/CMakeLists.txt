PROJECT(PMRSupportPlugin)

# Add the plugin

ADD_PLUGIN(PMRSupport
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../preferencesinterface.cpp

        src/pmrauthentication.cpp
        src/pmrexposure.cpp
        src/pmrsupportplugin.cpp
        src/pmrsupportpreferenceswidget.cpp
        src/pmrwebservice.cpp
        src/pmrwebservicemanager.cpp
        src/pmrwebserviceresponse.cpp
        src/pmrworkspace.cpp
        src/pmrworkspacefilenode.cpp
        src/pmrworkspacemanager.cpp
    HEADERS_MOC
        ../../preferencesinterface.h

        src/pmrauthentication.h
        src/pmrexposure.h
        src/pmrsupportplugin.h
        src/pmrsupportpreferenceswidget.h
        src/pmrwebservice.h
        src/pmrwebservicemanager.h
        src/pmrwebserviceresponse.h
        src/pmrworkspace.h
        src/pmrworkspacefilenode.h
        src/pmrworkspacemanager.h
    UIS
        src/pmrsupportpreferenceswidget.ui
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        ${LIBGIT2_PLUGIN}
        ${OAUTH_PLUGIN}
        WebViewerWidget
        zlib
    PLUGIN_BINARIES
        ${LIBGIT2_PLUGIN_BINARY}
        ${OAUTH_PLUGIN_BINARY}
)
