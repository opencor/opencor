PROJECT(UserMessagePlugin)

# Add the plugin

ADD_PLUGIN(UserMessage
    SOURCES
        ../../plugininfo.cpp

        src/usermessageplugin.cpp
        src/usermessagewidget.cpp
    HEADERS_MOC
        src/usermessageplugin.h
        src/usermessagewidget.h
    INCLUDE_DIRS
        src
    QT_MODULES
        Widgets
    QT_LIBRARIES
        QtCore
        QtGui
        QtWidgets
)
