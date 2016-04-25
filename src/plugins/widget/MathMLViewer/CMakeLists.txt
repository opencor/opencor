PROJECT(MathMLViewerPlugin)

# Retrieve Qwt's definitions

RETRIEVE_QWT_DEFINITIONS()

# Add the plugin

IF(USE_PREBUILT_QWT_PLUGIN)
    SET(XML Xml)
ELSE()
    SET(XML)
ENDIF()

ADD_PLUGIN(MathMLViewer
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/mathmlviewerplugin.cpp
        src/mathmlviewerwidget.cpp
    HEADERS_MOC
        src/mathmlviewerplugin.h
        src/mathmlviewerwidget.h
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
