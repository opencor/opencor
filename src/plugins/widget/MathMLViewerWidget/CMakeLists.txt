PROJECT(MathMLViewerWidgetPlugin)

# Add the plugin

ADD_PLUGIN(MathMLViewerWidget
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
