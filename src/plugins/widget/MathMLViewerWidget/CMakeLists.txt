project(MathMLViewerWidgetPlugin)

# Add the plugin

add_plugin(MathMLViewerWidget
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/mathmlviewerwidget.cpp
        src/mathmlviewerwidgetplugin.cpp
    HEADERS_MOC
        src/mathmlviewerwidget.h
        src/mathmlviewerwidgetplugin.h
    PLUGINS
        Core
        Qwt
)
