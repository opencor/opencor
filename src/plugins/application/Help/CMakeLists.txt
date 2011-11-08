PROJECT(HelpPlugin)

# Add the plugin

ADD_PLUGIN(Help
    SOURCES
        ../../coreinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/helpplugin.cpp
        src/helpwidget.cpp
        src/helpwindow.cpp
    HEADERS
        ../../coreinterface.h
        ../../guiinterface.h
        ../../i18ninterface.h
        ../../plugininfo.h
    HEADERS_MOC
        src/helpplugin.h
        src/helpwidget.h
        src/helpwindow.h
    UIS
        src/helpwindow.ui
    INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        Core
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtHelp
        QtNetwork
        QtWebKit
)
