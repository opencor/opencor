PROJECT(ZincWindowPlugin)

# Add the plugin

ADD_PLUGIN(ZincWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../windowinterface.cpp

        src/zincwindowplugin.cpp
        src/zincwindowwidget.cpp
        src/zincwindowwindow.cpp
    HEADERS_MOC
        src/zincwindowplugin.h
        src/zincwindowwidget.h
        src/zincwindowwindow.h
    UIS
        src/zincwindowwindow.ui
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        Zinc
)
