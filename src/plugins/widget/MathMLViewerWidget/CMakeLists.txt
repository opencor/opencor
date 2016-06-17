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
    INCLUDE_DIRS
        src
    PLUGINS
        Core
        ${QWT_PLUGIN}
    PLUGIN_BINARIES
        ${QWT_PLUGIN_BINARY}
)
