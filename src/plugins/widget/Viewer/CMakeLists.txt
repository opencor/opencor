PROJECT(ViewerPlugin)

# Apply Qwt's build settings

APPLY_QWT_BUILD_SETTINGS()

# Add the plugin

IF(USE_PREBUILT_QWT_PLUGIN)
    SET(XML Xml)
ELSE()
    SET(XML)
ENDIF()

ADD_PLUGIN(Viewer
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/viewerplugin.cpp
        src/viewerwidget.cpp
    HEADERS_MOC
        src/viewerplugin.h
        src/viewerwidget.h
    INCLUDE_DIRS
        src
    DEFINITIONS
        ${QWT_DEFINITIONS}
    PLUGINS
        Core
        ${QWT_PLUGIN}
    PLUGIN_BINARIES
        ${QWT_PLUGIN_BINARY}
    QT_MODULES
        ${XML}
)
