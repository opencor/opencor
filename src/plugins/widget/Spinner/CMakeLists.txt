PROJECT(SpinnerPlugin)

# Add the plugin

ADD_PLUGIN(Spinner
    SOURCES
        ../../plugininfo.cpp

        src/spinnerplugin.cpp
        src/spinnerwidget.cpp
    HEADERS_MOC
        src/spinnerplugin.h
        src/spinnerwidget.h
    INCLUDE_DIRS
        src
    QT_MODULES
        Widgets
    QT_LIBRARIES
        QtCore
        QtGui
        QtWidgets
)
