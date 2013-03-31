PROJECT(HelpPlugin)

# Generate the help files which will be embedded in the executable as a resource

EXECUTE_PROCESS(COMMAND ${QT_BINARY_DIR}/qcollectiongenerator ${CMAKE_SOURCE_DIR}/doc/${MAIN_PROJECT_NAME}.qhcp
                                                           -o ${CMAKE_BINARY_DIR}/${MAIN_PROJECT_NAME}.qhc)

# Add the plugin

ADD_PLUGIN(Help
    SOURCES
        ../../coreinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../interface.cpp
        ../../plugininfo.cpp

        src/helpplugin.cpp
        src/helpwidget.cpp
        src/helpwindow.cpp
    HEADERS_MOC
        src/helpplugin.h
        src/helpwidget.h
        src/helpwindow.h
    UIS
        src/helpwindow.ui
    INCLUDE_DIRS
        src
    OPENCOR_PLUGIN_DEPENDENCIES
        Core
    QT_MODULES
        Help
        Network
        WebKitWidgets
        Widgets
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtHelp
        QtNetwork
        QtOpenGL
        QtPrintSupport
        QtQml
        QtQuick
        QtWebKit
        QtWebKitWidgets
        QtWidgets
)
