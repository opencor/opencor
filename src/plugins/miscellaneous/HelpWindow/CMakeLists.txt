PROJECT(HelpWindowPlugin)

# Generate the help files which will be embedded in the executable as a resource

EXECUTE_PROCESS(COMMAND ${QT_BINARY_DIR}/qcollectiongenerator ${CMAKE_SOURCE_DIR}/doc/${CMAKE_PROJECT_NAME}.qhcp
                                                           -o ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.qhc)

# Add the plugin

ADD_PLUGIN(HelpWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/helpwindowplugin.cpp
        src/helpwindowwidget.cpp
        src/helpwindowwindow.cpp
    HEADERS_MOC
        src/helpwindowplugin.h
        src/helpwindowwidget.h
        src/helpwindowwindow.h
    UIS
        src/helpwindowwindow.ui
    INCLUDE_DIRS
        src
    PLUGINS
        Core
    QT_MODULES
        Help
        PrintSupport
        WebKitWidgets
    QT_LIBRARIES
        QtCore
        QtGui
        QtHelp
        QtNetwork
        QtPrintSupport
        QtWebKit
        QtWebKitWidgets
        QtWidgets
        QtXml
        QtXmlPatterns
)
