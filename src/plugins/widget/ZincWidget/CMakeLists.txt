PROJECT(ZincWidgetPlugin)

# Add the plugin

ADD_PLUGIN(ZincWidget
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/zincwidget.cpp
        src/zincwidgetplugin.cpp
    HEADERS_MOC
        src/zincwidget.h
        src/zincwidgetplugin.h
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        Zinc
    QT_MODULES
        OpenGL
)
