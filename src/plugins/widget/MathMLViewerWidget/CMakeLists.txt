project(MathMLViewerWidgetPlugin)

# Add the plugin

add_plugin(MathMLViewerWidget
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/mathmlviewerwidget.cpp
        src/mathmlviewerwidgetplugin.cpp
    PLUGINS
        Core
        Qwt
)
