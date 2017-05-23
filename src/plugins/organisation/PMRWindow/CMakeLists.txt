PROJECT(PMRWindowPlugin)

# Add the plugin

ADD_PLUGIN(PMRWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../preferencesinterface.cpp
        ../../windowinterface.cpp

        src/pmrwindowplugin.cpp
        src/pmrwindowwidget.cpp
        src/pmrwindowwindow.cpp
    HEADERS_MOC
        ../../preferencesinterface.h

        src/pmrwindowplugin.h
        src/pmrwindowwidget.h
        src/pmrwindowwindow.h
    UIS
        src/pmrwindowwindow.ui
    PLUGINS
        PMRSupport
)
