PROJECT(ViewerPlugin)

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

        src/viewerwidget.cpp
        src/viewerplugin.cpp
    HEADERS_MOC
        src/viewerwidget.h
        src/viewerplugin.h
    INCLUDE_DIRS
        src
    DEFINITIONS
        QWT_DLL
    PLUGINS
        Core
        ${QWT_PLUGIN}
    PLUGIN_BINARIES
        ${QWT_PLUGIN_BINARY}
    QT_MODULES
        ${XML}
    QT_LIBRARIES
        QtConcurrent
        QtCore
        QtGui
        QtNetwork
        QtOpenGL
        QtPrintSupport
        QtSvg
        QtWidgets
        QtXml
        QtXmlPatterns
)
