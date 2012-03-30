PROJECT(FileOrganiserPlugin)

# Add the plugin

ADD_PLUGIN(FileOrganiser
    SOURCES
        ../../coreinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../interface.cpp
        ../../plugininfo.cpp

        src/fileorganisermodel.cpp
        src/fileorganiserplugin.cpp
        src/fileorganiserwidget.cpp
        src/fileorganiserwindow.cpp
    HEADERS_MOC
        src/fileorganisermodel.h
        src/fileorganiserplugin.h
        src/fileorganiserwidget.h
        src/fileorganiserwindow.h
    UIS
        src/fileorganiserwindow.ui
    INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        Core
    QT_DEPENDENCIES
        QtCore
        QtGui
)
