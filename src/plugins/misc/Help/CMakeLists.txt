PROJECT(HelpPlugin)

# Generate the help files which will be embedded in the executable as a resource

EXECUTE_PROCESS(COMMAND ${QT_BINARY_DIR}/qcollectiongenerator ${CMAKE_SOURCE_DIR}/doc/${CMAKE_PROJECT_NAME}.qhcp
                                                           -o ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.qhc)

# Add the plugin

ADD_PLUGIN(Help
    SOURCES
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
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
    PLUGINS
        Core
    QT_MODULES
        Help
        Network
        WebKitWidgets
    QT_LIBRARIES
        QtCore
        QtGui
        QtHelp
        QtNetwork
        QtOpenGL
        QtPrintSupport
        QtQml
        QtQuick
        QtSensors
        QtWebKit
        QtWebKitWidgets
        QtWidgets
)
