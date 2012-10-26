PROJECT(CorePlugin)

# Add the plugin
# Note: in order to support OpenCOR actions (e.g. to allow OpenCOR to respond
#       to an action coming from the Help plugin), we need to know about the
#       QtSingleApplication class, so like for OpenCOR we have it directly in
#       the plugin rather than as a dependency (it's easier that way)...

IF(WIN32)
    SET(PLATFORM win)
ELSE()
    SET(PLATFORM unix)
ENDIF()

ADD_PLUGIN(Core
    SOURCES
        ../../../3rdparty/QtSingleApplication/src/qtlocalpeer.cpp
        ../../../3rdparty/QtSingleApplication/src/qtlockedfile.cpp
        ../../../3rdparty/QtSingleApplication/src/qtlockedfile_${PLATFORM}.cpp
        ../../../3rdparty/QtSingleApplication/src/qtsingleapplication.cpp

        ../../coreinterface.cpp
        ../../fileinterface.cpp
        ../../guiinterface.cpp
        ../../i18ninterface.cpp
        ../../interface.cpp
        ../../plugin.cpp
        ../../plugininfo.cpp
        ../../pluginmanager.cpp

        src/borderedwidget.cpp
        src/centralwidget.cpp
        src/collapsiblewidget.cpp
        src/commonwidget.cpp
        src/coreplugin.cpp
        src/coreutils.cpp
        src/dockwidget.cpp
        src/file.cpp
        src/filemanager.cpp
        src/organisationwidget.cpp
        src/toolbar.cpp
        src/treeview.cpp
        src/usermessagewidget.cpp
        src/widget.cpp
    HEADERS_MOC
        ../../../3rdparty/QtSingleApplication/src/qtlocalpeer.h
        ../../../3rdparty/QtSingleApplication/src/qtsingleapplication.h

        ../../plugin.h
        ../../pluginmanager.h

        src/borderedwidget.h
        src/centralwidget.h
        src/collapsiblewidget.h
        src/coreplugin.h
        src/dockwidget.h
        src/filemanager.h
        src/organisationwidget.h
        src/toolbar.h
        src/treeview.h
        src/usermessagewidget.h
        src/widget.h
    UIS
        src/centralwidget.ui
        src/usermessagewidget.ui
    INCLUDE_DIRS
        src
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtNetwork
)
