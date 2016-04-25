PROJECT(PMRWindowPlugin)

# Add the plugin

ADD_PLUGIN(PMRWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/pmrwindowplugin.cpp
        src/pmrwindowwidget.cpp
        src/pmrwindowwindow.cpp
    HEADERS_MOC
        src/pmrwindowplugin.h
        src/pmrwindowwidget.h
        src/pmrwindowwindow.h
    UIS
        src/pmrwindowwidget.ui
        src/pmrwindowwindow.ui
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        PMRSupport
)
